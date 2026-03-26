1. At the behest of the Secret Service, Xerox and many other printer manufacturers introduced tracking dots to their laser color printers which can only be seen in the Blue channel
2. Open image in GIMP or equivalent
3. For GIMP, go to Windows > Dockable Dialogs > Channels
4. Deselect Red & Green channels, leaving only Alpha and Blue
5. Realize that there are a bunch of perfectly-square dots with equal spacing between them
6. Realize that the dots are grouped into grids with consistent spacing between them
7. Find an uninterrupted grid that is easily visible (bottom-right corner is perfect for this)
8. Search for the provided keywords to find pages like [this](https://repository.root-me.org/St%C3%A9ganographie/EN%20-%20Decoding-Identifying-Printer-Information-CCC-2007.pdf) or [this](https://media.taricorp.net/p15-embar.pdf)
9. Use those papers to determine which columns correspond to the serial number and the value associated with each row
10. Translate the dots into a serial number

# Flag
* Type: `static`
* Value: `CTF{13731486}`