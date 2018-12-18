# Name: Jorge Fernando Flores Pinto                                           #
# ID: V00880059                                                               #
# SENG265, Summer 2017                                                        #
#                                                                             #
# phase2.py performs the second phase towards data compression. It encodes    #
# and decodes files to reduce their file size. phase2.py also manages run     #
# lengths greater than 127.                                                   #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

from Encoding import Encoding
from Decoding import Decoding

import sys, getopt, struct

#Turns characters into ASCII
def into_ascii(char_list):
    ascii = []
    for char in char_list:
        if (isinstance(char, int)):
            char += 0x80
            ascii.append(chr(char))
        else:
            ascii.append(char)
    return ascii

#Converts ASCII into a format the program can handle
def from_ascii(ascii_string):
    run_length_deco_list = []
    for char in ascii_string:
        val = ord(char)
        if (val >= 0x80):
            val -= 0x80
            run_length_deco_list.append(val)
        else:
            run_length_deco_list.append(char)

    return run_length_deco_list

def main():
    # https://docs.python.org/3.1/library/getopt.html
    try:
        opts, args = getopt.getopt(sys.argv[1:] , "edi:o:", 
            ["encode", "decode", "infile=", "outfile="])

    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    fn = None
    output = None
    command = None

    for o, a in opts:
        if o == "--encode":
            if (command == "decode"):
                print("Error 1: Two commands have been requested.")
                exit()
            command = "encode"
        elif o == "--decode":
            if(command == "encode"):
                print("Error 1: Two commands have been requested.")
            command = "decode"
        elif o == "--infile":
            fn = a
        elif o == "--outfile":
            output = a

    scanned_text = ""

    with open(fn, encoding="latin-1", mode="r") as fp:
        for line in fp:
            scanned_text += line

    first_chars = list(scanned_text[:4])

    ints = [ord(c) for c in first_chars]
    bytes = bytearray(ints)
    hex_chars_tuple = struct.unpack("I", bytes)

    first_chars_hex = hex_chars_tuple[0]

    if (command == "encode"):
        if (first_chars_hex != 0xefbebaab):
            print("Error 2: The file you entered is not a .ph1 file.")
            exit()
            
        block = scanned_text[4:8]
        enco = Encoding(scanned_text[8:])

        enco.mtf_encode()

        enco.run_length_encode()

        ascii_list = into_ascii(enco.run_length_list)

        joined_string = "".join(ascii_list)

        with open(output, encoding="latin-1", mode="w") as fp:
            fp.write("\xda\xaa\xaa\xad")
            fp.write(block)
            fp.write(joined_string)
    
    elif (command == "decode"):
        if (first_chars_hex != 0xadaaaada):
            print("Error 3: The file you entered is not a .ph2 file.")
            exit()

        block = scanned_text[4:8]
        deco = Decoding(scanned_text[8:])

        deco.run_length_list = from_ascii(deco.initial_string)
        deco.run_length_decode()

        deco.mtf_decode()

        result_string = "".join(deco.result_list)

        with open(output, encoding="latin-1", mode="w") as fp:
            fp.write("\xab\xba\xbe\xef")
            fp.write(block)

            fp.write(result_string)

if __name__ == '__main__':
    main()


