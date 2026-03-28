1. Look up JavaScript's Math.random() function to determine that it uses Xorshift128+
2. Look up ways to invert Math.random() and Xorshift128+ which will take you to a tutorial like [this](https://www.youtube.com/watch?v=XDsYPXRCXAs) or [this](https://littlemaninmyhead.wordpress.com/2025/08/31/inverting-the-xorshift128-random-number-generator/)
3. Use those tutorials to find a tool like [this xorshift128+ inverter](https://github.com/ScottContini/a_better_xorshift128-inverter)
4. See that these tools only take hex values
5. Convert 2725572247253476556 -> 0x25d32d84a734e4cc, 15591863164500473095 -> 0xd8616cf05a8c7507, 55102595811394477 -> 0xc3c384d98a43ad
6. Pass the hex values to the inverter tools in a command like `./better_inverter.out 0x25d32d84a734e4cc 0xd8616cf05a8c7507 0xc3c384d98a43ad`
7. The tool should return the two derived seeds `0xfeedfacecafebeef 0xdeadfa1169360463`

# Flag
* Type: `static`
* Value: `CTF{feedfacecafebeef_deadfa1169360463}`