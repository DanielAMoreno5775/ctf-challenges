1. Execute `binwalk bean-dip.jpg` to see that there is a zip archive in the file
2. Execute `binwalk -e bean-dip.jpg` to extract the zip archive which yields 18E78.zip
3. Sometimes, binwalk will also unzip the file. If not, execute `unzip 18E78.zip` (or whatever the zip file is called)
4. Execute `xxd -r -p layer5.hex > layer4` which yields layer4
    * The file is called layer5.hex so further hints at the number of layers and tells you which tool to use
5. Open `layer4` and realize that it is base64-encoded based on the character set
6. Execute `base64 -d layer4 > layer3` which yields layer3
7. Execute `file layer3` to determine that it is a tar file
8. Execute `tar -xf layer3.tar` which yields dip2.jpg.gz
9. Execute `gunzip dip2.jpg.gz` which yields dip2.jpg
10. Execute `strings dip2.jpg` to get the flag

# Flag
* Type: `static`
* Value: `CTF{D1d_y0u_3nj0y_my_D1p}`