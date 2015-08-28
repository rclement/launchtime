import sys
import subprocess
import argparse


def formatter(files, recursive):
    command = [
               'astyle',
               '--style=allman',
               '--indent=spaces=4',
               '--min-conditional-indent=0',
               '--attach-extern-c',
               '--add-brackets',
               '--max-code-length=80'
              ]

    if recursive:
        command.append('--recursive')

    for f in files:
        command.append(f)

    subprocess.call(command, stdout=sys.stdout)


def main():
    parser = argparse.ArgumentParser(description='Simple C/C++ code formatter using Artistic Style')
    parser.add_argument('files', nargs='+', metavar='files',
                        help='C/C++ source files to be formatted')
    parser.add_argument('--recursive', dest='recursive', action='store_true',
                        help='search for files recursively')
    args = parser.parse_args()

    formatter(args.files, args.recursive)


if __name__ == '__main__':
    sys.exit(main())

