# Hello Library example
This is a simple example on how to use Sancus modules in libraries. The example allows to build normal or thin archives. Simply build with
```bash
make all
make sim
```
to build and simulate a normal (thick) library. To perform the same tests with a thin library where the object files are only linked into the archive and not actually contained in it, simply set the ARCHIVE_MODE variable to thin:
```bash
make clean # remember to clean before changing archive type
make sim ARCHIVE_MODE=thin
```
