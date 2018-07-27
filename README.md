# VitaTweaks

A collection of small tweaks for the PS Vita

## 1. NoLockScreen

This tweak allows you to disable the lockscreen. Downside of this is however that the passcode control will be bypassed too.

### Changelog v2
- Also disable lockscreen at boot

Download [nolockscreen.suprx](https://github.com/TheOfficialFloW/VitaTweaks/releases/tag/NoLockScreen), edit `ux0:tai/config.txt` enable it in `*main` as follows:

```
*main
ux0:tai/nolockscreen.suprx
```

## 2. Download Enabler

This tweak allows you to download any content from the webbrowser. It also fixes a mistake in Sony's code where a HEAD request was made instead of a GET request.

Download [download_enabler.suprx](https://github.com/TheOfficialFloW/VitaTweaks/releases/tag/DownloadEnabler), edit `ux0:tai/config.txt` and enable it in `*main` as follows:

```
*main
ux0:tai/download_enabler.suprx
```

## 3. NoTrophyMsg

This tweak disables the trophy hint message when you launch application whose savedatas are linked to a different account. Useful if you constantly switch accounts.

Download [notrophymsg.suprx](https://github.com/TheOfficialFloW/VitaTweaks/releases/tag/NoTrophyMsg), edit `ux0:tai/config.txt` and enable it in `*main` as follows:

```
*main
ux0:tai/notrophymsg.suprx
```

## 4. Custom Warning

This tweak allows you to set a custom warning message instead of the epilepsy warning when you bootup with enso.

### Changelog v2
- Fixed crash in system applications

Download [custom_warning.suprx](https://github.com/TheOfficialFloW/VitaTweaks/releases/tag/CustomWarning), edit `ux0:tai/config.txt` and enable it in `*main` as follows:

```
*main
ux0:tai/custom_warning.suprx
```

Then write your custom text to `ux0:tai/custom_warning.txt` using **Unicode Little Endian** format (with BOM, the bytes `FF FE` at the beginning of your file). You can also have no text by creating the file with the BOM only.

## Support

Support me on [patreon](https://www.patreon.com/TheOfficialFloW) for more content
