'''Project 2 test environment'''
from sys import argv

def sgrep(pat: str, File, flag=None):
    #NO FLAG
    if flag is None:
        with open(File) as fileInput:
            for line in fileInput:
                if pat in line:
                    print(line)
    #IGNORE CASE DISTINCTIONS
    if flag == '-i':
        with open(File) as fileInput:
            for line in fileInput:
                if pat.lower() in line.lower():
                    print(line)
    #INVERT SENSE OF MATCHING TO SELECT NON-MATCHING LINES
    if flag == '-v':
        with open(File) as fileInput:
                for line in fileInput:
                    if pat not in line:
                        print(line)
    #SUPRESS NORMAL OUTPUT. INSTEAD PRINT COUNT OF MATCHING LINES
    if flag == '-c':
        with open(File) as fileInput:
                matchcount = 0
                for line in fileInput:
                    if pat in line:
                        matchcount += 1
                print(matchcount)
        
    return
'''
def main():
    argc = len(argv)
    for i in range(3, argc)
        sgrep(argv[2], argv[i], flag)
'''

def main():
    sgrep('the', 'Project2_file.txt', '-c')

main()
