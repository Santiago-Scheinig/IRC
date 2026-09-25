# Milestones

Joint session first, then A/B/C run in parallel once the contract
(this scaffold) is merged into `main`.

## M0 - Scaffold (done, this repo)
Interfaces frozen: `Message`, `ICommand`, `Client`, `Channel`, `Server`,
`CommandRegistry`, `Config`, `Replies`. Nobody edits these signatures
solo after this point - it's a three-way conversation.

## M1 - Registration
- [ ] A: finish `Server::handleClientReadable` - recv into buffer, split on CRLF
- [ ] A: `POLLOUT` flush + `disconnectClient` on hangup/error
- [ ] B: `Parser::parse` - full IRC line grammar
- [ ] B: `CommandRegistry::dispatch` - lookup + `ERR_UNKNOWNCOMMAND`
- [ ] B: `PassCommand`, `NickCommand`, `UserCommand` (registration state machine)
- [ ] B: `PingCommand` / `PongCommand`

## M2 - Channels & messaging
- [ ] C: `Channel::broadcast`
- [ ] C: `JoinCommand`
- [ ] C: `PartCommand`
- [ ] C: `PrivmsgCommand` (user-to-user and user-to-channel)
- [ ] C: `QuitCommand`

## M3 - Operators & modes
- [ ] C: `KickCommand`
- [ ] C: `InviteCommand`
- [ ] C: `TopicCommand`
- [ ] C: `ModeCommand` (i / t / k / o / l)

## M4 - Edge cases & hardening
- [ ] All: partial-send handling on slow clients
- [ ] All: malformed-line / flood resilience
- [ ] All: test against a real client (irssi / WeeChat / HexChat), not just `nc`
