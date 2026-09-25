# ircserv

IRC server, C++98, single `poll()` loop, no threads/fork.

## Build

```
make        # build
make re     # rebuild clean
make clean  # remove .o files
make fclean # remove .o files and the binary
```

## Layout

```
include/    headers - the shared contract, frozen after M0
src/        implementations
main.cpp    stays tiny: Config -> Server -> run()
```

See `MILESTONES.md` for the ticket breakdown and `.github/ISSUE_TEMPLATE`
for how a ticket should be scoped.

## Ownership at a glance

- **Track A** - `Server`, `Client`, the poll loop, socket plumbing
- **Track B** - `Parser`, `Message`, `CommandRegistry`, `Replies`, registration commands
- **Track C** - `Channel`, channel/messaging commands, operator commands

Each `ICommand` implementation is one class in one file under
`src/commands/` (create that folder on your first command ticket) -
that's what keeps three people's work mergeable without stepping on
each other's lines.
