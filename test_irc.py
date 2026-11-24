#!/usr/bin/env python3

import socket
import subprocess
import sys
import time
import select
import os


HOST = "127.0.0.1"
PASSWORD = "password"
SERVER_EXECUTABLE = "./ircserv"


class IRCClient:
    def __init__(self, host, port, nickname):
        self.nickname = nickname
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(3)
        self.sock.connect((host, port))
        self.buffer = b""
        self.lines = []

    def send(self, command):
        self.sock.sendall((command + "\r\n").encode())

    def expect(self, expected, timeout=3):
        deadline = time.time() + timeout

        while time.time() < deadline:
            for i, line in enumerate(self.lines):
                if expected in line:
                    self.lines.pop(i)
                    print(f"    RECEIVED: {line}")
                    return True

            remaining = deadline - time.time()
            ready, _, _ = select.select(
                [self.sock], [], [], min(remaining, 0.2)
            )

            if not ready:
                continue

            try:
                data = self.sock.recv(4096)
            except (socket.timeout, BlockingIOError):
                continue

            if not data:
                break

            self.buffer += data

            while b"\n" in self.buffer:
                raw, self.buffer = self.buffer.split(b"\n", 1)
                line = raw.decode(errors="replace").rstrip("\r")
                self.lines.append(line)

        return False

    def register(self):
        self.send(f"PASS {PASSWORD}")
        self.send(f"NICK {self.nickname}")
        self.send(f"USER {self.nickname} 0 * :{self.nickname} Test")

        for code in ("001", "002", "003", "004"):
            if not self.expect(f" {code} "):
                raise AssertionError(
                    f"{self.nickname}: Missing registration reply {code}"
                )

    def close(self):
        try:
            self.sock.close()
        except OSError:
            pass


def get_free_port():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((HOST, 0))
    port = sock.getsockname()[1]
    sock.close()
    return port


def test(description, function):
    print(f"\n[TEST] {description}")

    try:
        function()
        print("[PASS]")
        return True
    except (AssertionError, OSError, TimeoutError) as error:
        print(f"[FAIL] {error}")
        return False


def main():
    if not os.path.isfile(SERVER_EXECUTABLE):
        print("ERROR: ircserv not found. Run 'make' first.")
        return 1

    port = get_free_port()

    print("=" * 55)
    print("             FT_IRC AUTOMATED TESTS")
    print("=" * 55)

    server = subprocess.Popen(
        [SERVER_EXECUTABLE, str(port), PASSWORD],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.STDOUT
    )

    alice = None
    bob = None
    results = []

    try:
        # Wait for server startup.
        for _ in range(30):
            if server.poll() is not None:
                raise RuntimeError("Server exited unexpectedly.")

            try:
                test_socket = socket.create_connection(
                    (HOST, port), timeout=0.2
                )
                test_socket.close()
                break
            except OSError:
                time.sleep(0.1)
        else:
            raise RuntimeError("Server failed to start.")

        print(f"\nServer listening on {HOST}:{port}")

        def register_alice():
            nonlocal alice
            alice = IRCClient(HOST, port, "alice")
            alice.register()

        results.append(test(
            "Alice registration (PASS/NICK/USER)",
            register_alice
        ))

        if not results[-1]:
            raise RuntimeError("Alice registration failed.")

        def register_bob():
            nonlocal bob
            bob = IRCClient(HOST, port, "bob")
            bob.register()

        results.append(test(
            "Bob registration (multiple clients)",
            register_bob
        ))

        if not results[-1]:
            raise RuntimeError("Bob registration failed.")

        def alice_join():
            alice.send("JOIN #test")

            assert alice.expect(" JOIN #test"), \
                "Alice did not receive JOIN notification."

            assert alice.expect(" 331 "), \
                "Missing RPL_NOTOPIC."

        results.append(test("Channel creation and JOIN", alice_join))

        if not results[-1]:
            raise RuntimeError("Alice JOIN failed.")

        def bob_join():
            bob.send("JOIN #test")

            assert bob.expect(" JOIN #test"), \
                "Bob did not receive JOIN notification."

            assert bob.expect(" 331 "), \
                "Missing RPL_NOTOPIC."

            assert alice.expect(":bob!bob@", timeout=3), \
                "Alice did not receive Bob's JOIN broadcast."

        results.append(test("Multiple clients joining same channel", bob_join))

        if not results[-1]:
            raise RuntimeError("Bob JOIN failed.")

        def channel_message():
            alice.send("PRIVMSG #test :Hello Bob!")

            assert bob.expect(
                "PRIVMSG #test :Hello Bob!"
            ), "Bob did not receive Alice's message."

        results.append(test("Channel PRIVMSG delivery", channel_message))

    except Exception as error:
        print(f"\n[ERROR] {error}")

    finally:
        if alice:
            alice.close()

        if bob:
            bob.close()

        server.terminate()

        try:
            server.wait(timeout=3)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait()

    passed = sum(results)
    total = len(results)

    print("\n" + "=" * 55)
    print(f"RESULT: {passed}/{total} TESTS PASSED")
    print("=" * 55)

    return 0 if total > 0 and passed == total else 1


if __name__ == "__main__":
    sys.exit(main())
