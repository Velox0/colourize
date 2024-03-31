<div align="center">
<img height="20" src="https://img.icons8.com/dusk/64/wrench.png" alt="wrench"/>
Currently a work in progress

<img width="60%" src="images/banner.png"></img>

depends on [libclr](https://www.github.com/velox0/libclr)

<hr>
<p>colourise text based on string pattern</p>
</div>

# Overview

Colourize allows you to easily colourise text streams based on specified string patterns. It is designed to provide a simple yet powerful way to add custom colors to different parts of your text.

# Usage

The program looks for specific strings specified after certain arguments such as `--after`, `--from`, and `--reset-on` and formats them with the specified color. Here's an example:

```bash
stat image.png | clr red --after ": " green --reset-on "  "
```

This command outputs the standard input in red. After encountering `: `, it formats the subsequent string in green. It then resets to red after encountering two consecutive spaces `  `.

By default, the color definitions are read from `~/.config/colourize/preset`.

## Example

```bash
stat README.md | clr bright_red --after Access: 55ccff
```

<img src="./images/example.png"></src>

Valid colours include hexadecimal colours and `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`, `bright_black`, `bright_red`, `bright_green`, and so on.

# Customization

To customize color presets, you can modify the configuration file located at `~/.config/colourize/preset`. This file defines the default color mappings.

# Installation

Make sure you have [libclr](https://www.github.com/velox0/libclr) installed. Then run `./install.sh`.

# TODO

- [x] fix kmp implementation
- [ ] config file support
- [ ] `--match` option; only colour matching string

<em>Note: The tool is currently under development, and improvements are ongoing.</em>
