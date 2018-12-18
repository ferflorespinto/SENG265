#WORKING VERSION IN SSH

from sys import argv
#from sys import getopt
import getopt, sys
import struct

#ideas: Exception handling. If no blocksize provided, default to 20.
#transforming part of the document (for example, by calling --half)
#or by providing a number (ex: --reduce 128)

def forward(initial_text, output_name, block):
    block_len = int(block)
    result_index = 0
    result = ""
    while (result_index < len(initial_text)):
        #Adding end of text character
        if(len(initial_text) - result_index > block_len):
            block_lines = initial_text[result_index:result_index+block_len]
       
        else:
            block_lines = initial_text[result_index:len(initial_text)]
            block_len = len(initial_text) - result_index

        block_lines = block_lines + chr(3)

        #Rotations. Beginning forward transform
        i = 0
        text_matrix = []
        for element in block_lines:
            text_matrix.append(block_lines[i:block_len+1] + block_lines[0:i])
            i = i + 1

        #print(text_matrix)

        #Sorting matrix
        text_matrix.sort()

        #print("\nsorted matrix:")
        #print(text_matrix)

        #Creating transformed string
        for string in text_matrix:
            result += string[len(text_matrix) - 1]

        result_index += block_len
        #print(result)

   # print(result)
    #Output into a file
    file = open(output_name, encoding="latin-1", mode="w")
    file.write("\xab\xba\xbe\xef")

    bytes = struct.pack("I", int(block))
    chars = [chr(c) for c in bytes]
    #print(chars)
    for char in chars:
        file.write(str(char))

    file.write(result)
    file.close()

def f(string):
    return string[-1] == "\x03"

def backward(initial_text, output_name, block):
    block_len = int(block)
    result_index = 8
    result = ""
    final_result = ""
    while (result_index < len(initial_text)):
        if(len(initial_text) - result_index > block_len):
            initial_content = initial_text[result_index:result_index+block_len+1]

        else:
            initial_content = initial_text[result_index:]
            block_len = len(initial_text) - result_index


        #print("\ninitially: " + initial_content)
        #print(len(initial_content))
        block_list = []
        sorted_block_list = []
        for c in initial_content:
            block_list.append(c)

        sorted_block_list = list(block_list)
        #print(block_list)
        sorted_block_list.sort()
        #print(sorted_block_list)
        #print(block_list)

        for element in block_list[:-1]:
            last_column = []
            for line in sorted_block_list:
                last_column.append(line[-1])

            #print("\nthis is last_column:\n")
            #print(last_column)
            i = 0
            for line in block_list:
                block_list[i] += last_column[i]
                i = i + 1
           
            sorted_block_list = list(block_list)
            sorted_block_list.sort()

            #print(block_list)

        result_index += block_len
        result_index += 1
    
        F = filter(f, block_list)
        result = list(F)
        result_string = result[0]

        final_result += result_string[:-1]
   
    #print(final_result)
    
    file = open(output_name, encoding="latin-1", mode="w")
    file.write(final_result)
    file.close()

def main():
    # https://docs.python.org/3.1/library/getopt.html
    try:
        opts, args = getopt.getopt(sys.argv[1:] , "fbi:o:", ["forward", "backward", "infile=", "outfile=", "blocksize="])

    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    fn = None
    output = None
    command = None
    blocksize = 0
    print(opts)
    for o, a in opts:
        if o == "--forward":
            command = "forward"
            print("Forward transform")
        elif o == "--backward":
            command = "backward"
            print("Backward transform")
        elif o == "--infile":
            fn = a
            print("infile is..." + fn)
        elif o == "--outfile":
            output = a
            print("outfile is..." + a)
        elif o == "--blocksize":
            blocksize = a
            print("blocksize = " + blocksize)


    #fn = argv[]
    scanned_text = ""
    #open as text
    with open(fn, encoding="latin-1", mode="r") as fp:
        for line in fp:
            scanned_text += line

    if command == "forward":
        forward(scanned_text, output, blocksize)

    elif command == "backward":
        block_hex = list(scanned_text[4:8])
        print(block_hex)
        ints = [ord(c) for c in block_hex]
        bytes = bytearray(ints)
        block_tuple = struct.unpack("I", bytes)
        blocksize = block_tuple[0]
        print(blocksize)
        #blocksize = int(block_hex, 16)
        #print(blocksize)
        backward(scanned_text, output, blocksize)

if __name__ == '__main__':
    main()
