### Simple one-to-one chat client server in c.
---
#### If Cryptodev not-supported
do
	
	make simple

#### If Cryptodev suppoerted
do 

	make crypto

### Usage:

	./{crypto, simple}-server.exe
	./{crypto, simple}-client.exe [server domain-name] 38888(or other if changed pre-compilation)

###### Purpose: test encryption functionality on virtual machine that supports cryptodev via virtqueue
