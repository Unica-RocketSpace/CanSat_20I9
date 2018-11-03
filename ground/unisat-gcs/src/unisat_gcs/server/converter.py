z = open('U86.txt', 'r')
l = open('U86_bin.txt', 'w')

line = z.readline()
al = len(line) * 16886
print(al)
i = 0
bin_char = ''
bin_str = ''
for i in range(al):
    char = z.read(i)
    for foo in range(len(char)):

        if char[foo] == '0':
             bin_char = '0000'
             bin_str += bin_char

        elif char[foo] == '1':
             bin_char = '0001'
             bin_str += bin_char

        elif char[foo] == '2':
             bin_char = '0010'
             bin_str += bin_char

        elif char[foo] == '3':
             bin_char = '0011'
             bin_str += bin_char

        elif char[foo] == '4':
             bin_char = '0100'
             bin_str += bin_char

        elif char[foo] == '5':
             bin_char = '0101'
             bin_str += bin_char

        elif char[foo] == '6':
             bin_char = '0110'
             bin_str += bin_char

        elif char[foo] == '7':
             bin_char = '0111'
             bin_str += bin_char

        elif char[foo] == '8':
             bin_char = '1000'
             bin_str += bin_char

        elif char[foo] == '9':
             bin_char = '1001'
             bin_str += bin_char

        elif char[foo] == 'a':
             bin_char = '1010'
             bin_str += bin_char

        elif char[foo] == 'b':
             bin_char = '1011'
             bin_str += bin_char

        elif char[foo] == 'c':
             bin_char = '1100'
             bin_str += bin_char

        elif char[foo] == 'd':
             bin_char = '1101'
             bin_str += bin_char

        elif char[foo] == 'e':
             bin_char = '1110'
             bin_str += bin_char

        elif char[foo] == 'f':
             bin_char = '1111'
             bin_str += bin_char

        elif char[foo] == ' ':
             bin_char = ' '
             bin_str += bin_char

        l.write(bin_str)
        print(bin_str)
        bin_str = ''
l.close()
z.close()











