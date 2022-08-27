import sys
argv = sys.argv

def findPattern() -> int:
    '''This function is used to find the index position of the 
       pattern argument in argv, assuming that there is one.
       The index position of the pattern will be used to determine
       the number of files to read from, or if we need to read 
       from standard input.
    '''
    global argv
    flag = (False, 0)
    argc = len(argv)
    for x in range(argc):
        if argv[x][0] == '-' and len(argv[x]) <= 4:
            flag = (True, x)
        if flag[0] == True and x != flag[1]:
            return x
    return 1


def findFlag() -> bool:
    """Looks through the arguments in argv and checks to 
       see if any of them are a flag. Returns one string that 
       contains the letters of any flags found. If no flags are
       found, an empty string is returned.
    """
    global argv
    i = False
    v = False
    c = False
    for x in range(len(argv)):
        if argv[x][0] == '-' and len(argv[x]) < 5:
            if 'i' in argv[x]:
                i = True
            if 'v' in argv[x]:
                v = True
            if 'c' in argv[x]:
                c = True
    if i and v and c:
        return 'ivc'
    elif i and v:
        return 'iv'
    elif i and c:
        return 'ic'
    elif v and c:
        return 'vc'
    elif i:
        return 'i'
    elif v:
        return 'v'
    elif c:
        return 'c'
    else:
        return ''


def sgrep(flag: str, pattern: str, File: str, fileCount: int) -> None:
    """This is the base sgrep function which calls sgrepC, 
       sgrepN, and sgrepIV to take care of the different operations
       that are supported by sgrep and its flags.
    """
    if fileCount > 1:
        name = File + ':'
    else:
        name = ''

    if flag == '':
        sgrepN(pattern, File, name)
    
    elif 'c' in flag:
        sgrepC(flag, pattern, File, name)
    
    else:
        sgrepIV(flag, pattern, File, name)
    return None


def sgrepN(pat: str, File: str, filename: str) -> None:
    """This function handles the case in which there is no
       flag provided (Standard sgreg operation).
    """
    if len(File) == 0:
        for line in sys.stdin:
            if pat in line:
                sys.stdout.write(f"{filename}{line}")
    else:
        try:
            with open(File) as f:
                for line in f:
                    if pat in line:
                        sys.stdout.write(f"{filename}{line}")
        except Exception:
            sys.stdout.write(f"sgrep.py: {File}: No such file or directory\n")

    return None


def sgrepC(flag: str, pat: str, File: str, filename: str) -> None:
    """The purpose of this function is to handle the -c -ic
       -vc and -ivc flags for the sgrep function. 
    """
    count = 0
    if len(File) == 0:
        for line in sys.stdin:
            if flag == 'c':
                if pat in line:
                    count +=1
            elif flag == 'ic':
                if pat.lower() in line.lower():
                    count += 1
            elif flag == 'vc':
                if pat not in line:
                    count += 1
            elif flag == 'ivc':
                if pat.lower() not in line.lower():
                    count += 1
    else:
        try:
            with open(File) as f:
                for line in f:
                    if flag == 'c':
                        if pat in line:
                            count +=1
                    elif flag == 'ic':
                        if pat.lower() in line.lower():
                            count += 1
                    elif flag == 'vc':
                        if pat not in line:
                            count += 1
                    elif flag == 'ivc':
                        if pat.lower() not in line.lower():
                            count += 1
        except Exception:
            sys.stdout.write(f"sgrep.py: {File}: No such file or directory\n")
            return None
                        
    sys.stdout.write(f"{filename}{count}\n")
    return None


def sgrepIV(flag: str, pat: str, File: str, filename: str) -> None:
    """The purpose of this function is to handle any of the 
       -i -v or -iv flags for the sgrep function.
    """
    if len(File) == 0:
        for line in sys.stdin:
            if flag == 'i':
                if pat.lower() in line.lower():
                    sys.stdout.write(f"{filename}{line}")
            if flag == 'v':
                if pat not in line:
                    sys.stdout.write(f"{filename}{line}")
            if flag == 'iv':
                if pat.lower() not in line.lower():
                    sys.stdout.write(f"{filename}{line}")

    else:
        try:
            with open(File) as f:
                for line in f:
                    if flag == 'i':
                        if pat.lower() in line.lower():
                            sys.stdout.write(f"{filename}{line}")
                    if flag == 'v':
                        if pat not in line:
                            sys.stdout.write(f"{filename}{line}")
                    if flag == 'iv':
                        if pat.lower() not in line.lower():
                            sys.stdout.write(f"{filename}{line}")
        except Exception:
            sys.stdout.write(f"sgrep.py: {File}: No such file or directory\n")

    return None


def main():
    global argv
    argc = len(argv)
    if argc < 2:
        sys.stderr.write('Usage: sgrep.py [OPTIONS] STRING [FILE ...] \n')
        return None

    flag = findFlag()
    pat_index = findPattern()
    fileCount = argc - (pat_index + 1)

    if fileCount == 0:
        sgrep(flag, argv[pat_index], '', 0)
    else:
        for i in range((pat_index + 1), argc):
            sgrep(flag, argv[pat_index], argv[i], fileCount)
    
    return None


if __name__ == "__main__":
    main()
