1. `echo "CTF{D1d_y0u_3nj0y_my_D1p}" >> dip2.jpg`
2. `gzip -k dip2.jpg`
3. `tar -cf layer3.tar dip2.jpg.gz`
4. `base64 layer3.tar > layer4.b64`
5. `xxd -p layer4.b64 > layer5.hex`
6. `zip layer6.zip layer5.hex`
7. `cat layer6.zip >> final.jpg`