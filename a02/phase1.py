# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#                                                                             #
#   Name: Jorge Fernando Flores Pinto                                         #
# ID: V00880059                                                               #
# SENG265, Summer 2017                                                        #
#                                                                             #
# phase1.py performs the first phase towards data compression – it performs   #
# forward and backward transforms on text files.                              #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

#!/usr/bin/env python3
from sys import argv
import getopt, sys, struct

# Performs the forward transform on a file. 
def forward(initial_text, output_name, block, reduced):
    block_len = int(block)
    reduced_length = int(reduced)
    result_index = 0
    result = ""
    reduced_flag = False
    final_step = False
    if (reduced_length > 0 and reduced_length < len(initial_text)):
        reduced_flag = True

    while (result_index < len(initial_text)):
        if(len(initial_text) - result_index > block_len):
            block_lines = initial_text[result_index:result_index+block_len]
       
        else:
            block_lines = initial_text[result_index:len(initial_text)]
            block_len = len(initial_text) - result_index

        if(reduced_flag and (reduced_length - result_index < block_len)):
            block_lines = initial_text[result_index:reduced_length]
            block_len = reduced_length - result_index
            final_step = True

        # Adding the end of text character to be able to do backward transform
        block_lines = block_lines + chr(3)

        # Rotations of the string
        i = 0
        text_matrix = []
        for element in block_lines:
            text_matrix.append(block_lines[i:block_len+1] + block_lines[0:i])
            i = i + 1

        text_matrix.sort()

        # Creating transformed string
        for string in text_matrix:
            result += string[len(text_matrix) - 1]

        result_index += block_len

        # If user provided a reduced size, loop will finish when the last step
        # is reached.
        if(final_step):
            break

    # Output into a file
    file = open(output_name, encoding="latin-1", mode="w")
    file.write("\xab\xba\xbe\xef")

    bytes = struct.pack("I", int(block))
    chars = [chr(c) for c in bytes]
    for char in chars:
        file.write(str(char))

    file.write(result)
    file.close()

#Function to use to filter the document
def f(string):
    return string[-1] == "\x03"

#Perform the backward transform on a file
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


        block_list = []
        sorted_block_list = []
        for c in initial_content:
            block_list.append(c)

        sorted_block_list = list(block_list)
        sorted_block_list.sort()

        for element in block_list[:-1]:
            last_column = []
            for line in sorted_block_list:
                last_column.append(line[-1])

            i = 0
            for line in block_list:
                block_list[i] += last_column[i]
                i = i + 1
           
            sorted_block_list = list(block_list)
            sorted_block_list.sort()

        result_index += block_len
        result_index += 1
    
        F = filter(f, block_list)
        result = list(F)
        result_string = result[0]

        final_result += result_string[:-1]
   
    
    file = open(output_name, encoding="latin-1", mode="w")
    file.write(final_result)
    file.close()

def main():
    # https://docs.python.org/3.1/library/getopt.html
    try:
        opts, args = getopt.getopt(sys.argv[1:] , "fbi:o:r:", ["forward", "backward", "infile=", "outfile=", "blocksize=", "reduce="])

    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    fn = None
    output = None
    command = None
    blocksize = 0
    reduce_num = 0

    for o, a in opts:
        if o == "--forward":
            command = "forward"
        elif o == "--backward":
            command = "backward"
        elif o == "--infile":
            fn = a
        elif o == "--outfile":
            output = a
        elif o == "--blocksize":
            blocksize = a
        elif o == "--reduce":
            reduce_num = a

    scanned_text = ""

    with open(fn, encoding="latin-1", mode="r") as fp:
        for line in fp:
            scanned_text += line

    if command == "forward":
        forward(scanned_text, output, blocksize, reduce_num)

    elif command == "backward":
        block_hex = list(scanned_text[4:8])
        ints = [ord(c) for c in block_hex]
        bytes = bytearray(ints)
        block_tuple = struct.unpack("I", bytes)
        blocksize = block_tuple[0]

        backward(scanned_text, output, blocksize)

if __name__ == '__main__':
    main()