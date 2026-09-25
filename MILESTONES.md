# Milestones

Joint session first, then A/B/C run in parallel once the M0 is merged into `main`.

## M0 - Scaffold
Include frozen: `Message`, `ICommand`, `Client`, `Channel`, `Server`,
`CommandRegistry`, `Config`, `Replies`. Nobody edits these signatures
solo after this point - it's a three-way conversation.

## M1 - Registration
- [ ] A:
- [ ] B:
- [ ] C:

## M2 - Channels & messaging
- [ ] A:
- [ ] B:
- [ ] C:

## M3 - Operators & modes
- [ ] A:
- [ ] B:
- [ ] C:

## M4 - Edge cases & hardening
- [ ] All: partial-send handling on slow clients
- [ ] All: malformed-line / flood resilience
- [ ] All: test against a real client (irssi / WeeChat / HexChat), not just `nc`
