# libhijacker

Project name is temporary

This repo will likely be short lived. I got tired of waiting and wanted to upload it somewhere.
Proper credits will be attributed later.

**Fork with support for game patching. check out the [original project](https://github.com/astrelsky/libhijacker) by astrelsky. (more for developers).**

# Supported Firmwares

- 3.00
- 3.10
- 3.20
- 4.00
- 4.02 (needs checking)
- 4.03
- 4.50
- 4.51

# Features Not Available in Scriptless and Rest Mode Build

- Reloadable daemon
- FTP

# Quick Setup

- Download the latest zip from the [release](https://github.com/illusion0001/libhijacker/releases/latest) page.
- Extract the zip with all files to a folder on your computer.
- Load your preferred method of exploiting your PS5. (BDJB or webkit).
- On ELF loader stage, send payload using netcat.

```bash
nc -q0 (ps5 ip) 9020 bin/spawner.elf
```

# Button Combos

### Home Screen

- Press Square and Triangle to toggle patch apply state.
- Press Square and Circle to toggle fliprate apply state.
  - **Note: Games that have custom frame rate limiter will not work with this patch.**
  - Share results with working games in this [ticket](https://github.com/illusion0001/libhijacker/issues/14)

# Supported Titles

### PS4

- Batman: Arkham Knight v1.15: 60 FPS
- Bloodborne v1.09: 60 FPS by Lance McDonald
  - Notes: You may encounter softlock during Laurence (optional DLC boss) cinematic. Disable patch to progress.
  - Also includes:
    - No Motion Blur
    - No Chromatic Aberration
- Burnout Paradise Remastered v1.03
  - Skip Logo Videos
  - Skip First Time Introduction Video
- Crash Team Racing v1.21: 60 FPS
- Dragon Age: Inquisition v1.12: 60 FPS
- Driveclub v1.28: 60 FPS
- Gravity Rush 2 v1.11: 60 FPS
- The Order 1886: v1.02: 60 FPS
  - Notes: Softlocks will occur in Chapter 6 and 9 during Quick Time Events. Disable patch to progress.
- The Last Guardian v1.03: 4K at 60FPS
- The Last of Us Remastered 60 FPS + Debug Menu (v1.11)
- The Last of Us Part 2 60 FPS + Debug Menu (v1.00 + v1.09)
- Uncharted: The Nathan Drake Collection Debug Menu (v1.00)
- Uncharted 4: A Thief's End 60 FPS (v1.33)
- Uncharted: The Lost Legacy 60 FPS (v1.09)
- Just Cause 4 v1.05: 60 FPS
- Just Cause 4 v1.32: 60 FPS
- Shadow of the Colossus: 1440p at 60FPS + Debug Menu
- Silent Hills: P.T at 60FPS
- Red Dead Redemption 2 v1.29: 60FPS
- Mafia Definitive Edition v1.04: 60 FPS + Skip logo movies

### PS5

- Demon Souls v1.00
  - 60 FPS Unlock in Cinematic Mode
  - Debug Menu
- Demon Souls v1.04
  - Debug Menu
- Uncharted: Legacy of Thieves Collection v1.00
  - Debug Menu

# Supported Titles (120Hz)

| Games                                              | Enabled by default                      | Notes                                                                                                                            |
|----------------------------------------------------|-----------------------------------------|----------------------------------------------------------------------------------------------------------------------------------|
| Battlefield 4 v1.24                                | Yes                                     | -                                                                                                                                |
| Bloodborne v1.09                                   | No (Hold Cross Button during Launch)    | Does not reach stable 120FPS.                                                                                                    |
| Burnout Paradise Remastered v1.03                  | No (Hold Cross Button during Launch)    | Has game breaking vehicle steering issues.                                                                                       |
| Gravity Rush Remastered v1.00                      | Yes                                     | -                                                                                                                                |
| Gravity Rush 2 v1.11                               | No (Hold Cross Button during Launch)    | Does not reach stable 120FPS<br>Fast cameras, broken in-game cinematics.                                                         |
| Demon Souls v1.00                                  | No (Hold Cross Button during Launch)    | Does not reach stable 120FPS<br>Half speed in Cinematic Mode.<br>Use performance mode in Graphics Options for proper game speed. |
| Doom (2016) v1.12                                  | Yes                                     | -                                                                                                                                |
| Mirror's Edge Catalyst v1.02                       | Yes                                     | -                                                                                                                                |
| NieR Replicant v1.03                               | Yes                                     | -                                                                                                                                |
| The Last Guardian v1.03                            | No (Hold Cross Button during Launch)    | Does not reach stable 120FPS.                                                                                                    |
| The Last of Us Remastered v1.11                    | Yes                                     | Further testing required.                                                                                                        |
| The Last of Us Part 2 v1.09                        | No (Hold Cross Button during Launch)    | Does not reach stable 120FPS.<br>Broken Camera in some scripted scenes.<br>Graphical Artifacts in 1080p mode.                    |
| Shadow of the Colossus v1.00/v1.01                 | Yes (Use Performance Mode in Game Menu) | -                                                                                                                                |
| Uncharted: The Nathan Drake Collection v1.00/v1.02 | Yes                                     | -                                                                                                                                |
| Killzone: Shadow Fall v1.81                        | No (Hold Cross Button during Launch)    | Does not reach stable 120FPS.                                                                                                    |

# XML Config

Version 1.152 now supports XML loading. Files are located in `/data/` after first load of payload.

### `game_patch_cfg.xml`

Contains keys to toggle hardcoded patches.

```xml
<cfg>
    <!-- Supported key value: `true`, `false`, `1`, `0` -->
    <GravityRush1_English>0</GravityRush1_English>
    <GravityRush2_60FPS>0</GravityRush2_60FPS>
    <Bloodborne_60FPS>1</Bloodborne_60FPS>
    <Bloodborne_MotionBlur>0</Bloodborne_MotionBlur>
    <Bloodborne_ChromaticAberration>0</Bloodborne_ChromaticAberration>
    <Bloodborne_DebugCamera>0</Bloodborne_DebugCamera>
    <Bloodborne_ColorBorderDiag>0</Bloodborne_ColorBorderDiag>
    <Bloodborne_Vsync>0</Bloodborne_Vsync>
    <Bloodborne_1280x720>0</Bloodborne_1280x720>
    <SOTC_DebugMenu>0</SOTC_DebugMenu>
    <TheOrder1886_60FPS>1</TheOrder1886_60FPS>
    <TheOrder1886_16_9>0</TheOrder1886_16_9>
    <Uncharted_4_Remaster_DebugMenu>0</Uncharted_4_Remaster_DebugMenu>
    <Uncharted_TheLostLegacy_Remaster_DebugMenu>0</Uncharted_TheLostLegacy_Remaster_DebugMenu>
    <DemonSouls_UnlockFPS>0</DemonSouls_UnlockFPS>
    <DemonSouls_DebugMenu>0</DemonSouls_DebugMenu>
    <DemonSouls104_DebugMenu>0</DemonSouls104_DebugMenu>
    <Driveclub_60FPS>1</Driveclub_60FPS>
    <Driveclub_UnlockAllCarsBikes>1</Driveclub_UnlockAllCarsBikes>
    <TheLastGurdian_60FPS>0</TheLastGurdian_60FPS>
    <TheLastOfUs2_109_Force1080p_in_120Hz>0</TheLastOfUs2_109_Force1080p_in_120Hz>
    <BurnoutParadise_103_120hz>0</BurnoutParadise_103_120hz>
    <BurnoutParadise_103_1920x1080>1</BurnoutParadise_103_1920x1080>
    <BurnoutParadise_103_SkipLogos>1</BurnoutParadise_103_SkipLogos>
    <BurnoutParadise_103_SkipIntroVideo>0</BurnoutParadise_103_SkipIntroVideo>
    <UCC_DebugMenu>0</UCC_DebugMenu>
</cfg>
```

### `game_patch_fliprate_list.xml`

This [file](data/game_patch_fliprate_list.xml) contains list of Title IDs that is known to work with Universal Fliprate Unlock.

Example:

```xml
<FliprateList>
    <TitleID>
        <!-- Unlock FPS for `CUSA00001`+`CUSA00002` (PS4 Titles) -->
        <ID>CUSA00001</ID>
        <ID>CUSA00002</ID>
        <!-- Unlock FPS for `PPSA00001`+`PPSA00002` (PS5 Titles) -->
        <ID>PPSA00001</ID>
        <ID>PPSA00002</ID>
    </TitleID>
</FliprateList>
```

# Known Issues (specific to this fork)

- Daemon sometimes crashes when scrolling through the game list.
- Daemon may crash with nullptr dereference in game patch thread.
- Daemon may reapply patch while game is running.

# Building

1. Install Clang + LLVM 14+.
2. Build and install [PS5SDK](https://github.com/PS5Dev/PS5SDK) (firmware does not matter).
3. Clone this repo with recursive submodules.
4. Generate libary stubs using stubber which can be found in  [`/stubber`](/stubber) folder.
   - Build `main.go`: `go build`
   - Dump `/system/lib/common/` folder from your console in decrypted prx files using the [PS5-SELF-Decrypter](https://github.com/Cryptogenic/PS5-SELF-Decrypter) payload from SpecterDev.
   - Or use my [pre generated stubs](https://github.com/illusion0001/libhijacker/releases/tag/stubber-lib).
   - Run the stubber. `./main aerolib.csv (PS5 System Folder) (Output Folder)`
   - Copy `.so` files to this repository folder into `lib`. (Create one if it does not exist)
5. Build the hijacker

```bash
# this assume you have llvm 16
cmake -G Ninja -DCMAKE_C_COMPILER=clang-16 -DCMAKE_CXX_COMPILER=clang++-16 -DCMAKE_TOOLCHAIN_FILE=$PS5SDK/cmake/toolchain-ps5.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .
ninja
```

6. Load it on your console using `send_elf.py`. you will see kernel log output on the first instance where you originally send the spawner payload.
   - `python3 send_elf.py (PS5 IP)`

# Special Thanks

To all testers that signed up for game patch beta test!

- ac2pic
- ashenContinuum
- Cerebral-Context
- Cameron
- CrystalCircus
- DrChrisRespect
- embee
- erdosadam24
- hobbo0408
- Lustcyna
- maplemiyazaki
- jrson
- Lord Maximus Farquaad
- iknowcpp
- peekurchoo
- pinoy24
- sirbri83
- razzysxpb
- rudiano
- Voredy
- VIPO777
- WardFail
- Whitehawkx
