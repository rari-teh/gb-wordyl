# gb-wordyl

A Wordle clone for the Nintendo Game Boy / Color and Analogue Pocket!

This is a mostly re-written and greatly expanded fork (by [bbbbbr](https://github.com/bbbbbr/gb-wordle)) of the original version by [stacksmashing](https://github.com/stacksmashing/gb-wordle)  ([twitter](http://twitter.com/ghidraninja)).

It adds a full dictionary (versus the bloom filter), thousands more solution words, multiple dictionary languages, Game Boy Color support, and many other features.


### Play Online
Playable online at: https://bbbbbr.itch.io/gb-wordyl

Downloads: See Releases for ROMS or at [itch.io]( https://bbbbbr.itch.io/gb-wordyl)


### Credits and Thanks

UI Language Translation credits:
  - German: [Skite2001(https://twitter.com/skite2001)
  - Dutch: [Ferrante Pescara](https://ferrantecrafts.com)

Additional improvements from:
  - [arpruss](https://github.com/arpruss/gb-fiver) : Highlighting fixes, Improved Dictionary compression and lookup speed
  - [zeta_two](https://github.com/ZetaTwo/gb-wordle) : Previous dictionary compression


### Features
  - Game stats: Won, Lost, Streak & Win Percentage (since power on)
  - Hard mode (with auto-fill of previous exact matches)
  - Full official English answer word list and dictionary less a couple cringy words (~12,900 words)
  - All in a 32K ROM
  - Controls:
    - A: Add Letter
    - B: Remove Letter
    - START: Submit guess
    - SELECT + B: Move Board Cursor Left
    - SELECT + A:  Move Board Cursor Right
    - SELECT + START: Auto-fill exact matches of previous guesses
    - 3 x SELECT: Options menu (Stats, Reset Stats, Forfeit Round)
  - Multiple language dictionaries (different ROM for each)
    - Deutsch (DE), English (EN), Español (ES), Français (FR), Italiano (IT), Nederlands (NL), Latin (LA)
    - No words with special chars, just English A-Z letters    


Built using [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020)

![GB-Wordyl gameplay](/info/gb-wordyl_demo.gif)
![GB-Wordyl gameplay](/info/gb-wordyl_demo_cgb.gif)

