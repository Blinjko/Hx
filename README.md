
# Hx

A simple command line program for creating and reversing hexdumps.

## Building

The only requirement for building is POSIX compatible libc, and a compiler of course.
To build, you can use the makefile, or run the compile command itself. The makefile contains
two different build options, one for GCC, and one for Clang. If you're on MS Windows, you will
just have to clone the repository and run the compile command yourself. The compile command for
GCC looks like ```gcc main.c functions.c -O2 -o Hx```, the Clang one is also very similar.

## Usage

Syntax: ```Hx {ACTION} <FILE> [OPTIONS]```, yes the order matters. An action is mandatory, as with a file to operate upon, but options are not mandatory.  

An exhaustive list of actions, and options: ```Hx {dump | rebuild} <file> [-S <value>, -E <value>, -B <value>, -P, -NS]```.  

When doing a hexdump, the ```dump``` action should be used, and the input will be ```<file>```, do note that to use ```stdin``` as input, use the ```-``` character in place of a filename.  
When doing a hexdump the options are defined as so:
- ```-S <value>``` the start offset. This determines the start offset of the hexdump, that is to say,```<value>``` number of bytes will be skipped from the beginning of the file. Default = 0
- ```-E <value>``` the end offset. Similar to start offset, the supplied value here determines when to stop the hexdump. Default = EOF
- ```-B <value>``` number of hexidecimal bytes to display on a singular line. Default = 16
- ```-P``` execute a plain dump. A plain dump has no offset marker, no text view, and no spaces between the hexidecimal bytes. Note, this option will override ```-NS```. Default is to not do a plain dump.
- ```-NS``` No spaces, don't include spaces inbetween the hexidecimal bytes, the default is to include spaces inbetween the hexidecimal bytes.

Some things to note are, the values passed to any of the options must be positive, and the end offset cannot be less than the start offset, or visa versa.  

To rebuild / reconstruct / reverse a hexdump, the ```rebuild``` action should be used. Some key things to note when rebuilding a hexdump are, if ```-B, -P, -NS``` were used in creating the hexdump, they need to be used when reversing it, ```<file>``` is where the hexdump will be read from, use ```-``` for ```stdin```, and finally ```-S and -E``` determine what hexidecimal byte to start and end at respectively.

## Examples

1. Take a hexdump of the program itself, ```Hx dump Hx```
2. Take a hexdump of the first 16 bytes of the program, ```Hx dump Hx -E 16```
3. Take a hexdump of a certain section of the program, ```Hx dump Hx -S 680 -E 707```
4. Read bytes from stdin and put the hexdump in a file, ```Hx dump - > hex_dump_output```
5. Do a normal hexdump, just with more bytes per line, ```Hx dump Hx -B 32```
6. Do a plain hexdump, with 32 bytes per line, ```Hx dump Hx -B 32 -P```  

Now the commands to rebuild the hexdumps above, assuming their outputs were put into a file called ```output```.
1. ```Hx rebuild output```
2. ```Hx rebuild output```
3. ```Hx rebuild output```
4. ```Hx rebuild output```
5. ```Hx rebuild output -B 32```
5. ```Hx rebuild output -B 32 -P```  

Straightforward enough, eh?


## Misc

As far as commiting / submitting changes goes, I will attempt to be ontop of it, but please forgive me if I'm not, after all there's just soooo much traffic... Also, any advice / citisisms are welcome! I'm always interested in ways I can improve.
