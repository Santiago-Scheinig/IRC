#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// Owner: track C (channels/messaging).
// Owns its own membership and broadcasting -- commands ask the Channel to
// do things rather than reaching into its internals, so channel-mode
// logic (i/t/k/o/l) stays in one place instead of leaking into every
// command that touches a channel.

#endif
