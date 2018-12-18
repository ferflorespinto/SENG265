# Name: Jorge Fernando Flores Pinto                                           #
# ID: V00880059                                                               #
# SENG265, Summer 2017                                                        #
#                                                                             #
# The class Decoding.py performs the decoding step for phase2.py. It takes    #
# a .ph2 file as input and does all necessary steps to output an decoded      #
# .ph1 file.                                                                  #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

import struct

class Decoding:
    def __init__(self, initial_string):
        self.initial_string = initial_string
        self.dict = {}
        self.swapped_dict = {}
        self.mtf_list = []
        self.run_length_list = []
        self.result_list = []

    #Just like int the encoding step, it moves a character to the first position of the 
    #list and shifts all characters before it by one
    def _swap(self, key_to_swap):
        if (self.dict[key_to_swap] != 1):
            for key in self.dict:
                if (self.dict[key] < len(self.dict) and self.dict[key] < self.dict[key_to_swap]):
                    self.dict[key] += 1

            self.dict[key_to_swap] = 1

    #Gets the character (key) in the dictionary from a given value
    def _get_value(self, num):
        for char in self.dict:
            if (self.dict[char] == num):
                return char

    #Performs the Move to Front decoding
    def mtf_decode(self):
        loop_counter = 0
        next_elem = None
        previous_value = 0
        list_size = len(self.mtf_list)

        for char in self.mtf_list:
            loop_counter += 1
            if (loop_counter < list_size):
                next_elem = self.mtf_list[loop_counter]

            if(isinstance(char, int) == True):
                if (isinstance(next_elem, int) == False):
                    previous_value = char
                else:
                    self.result_list.append(self._get_value(char))
                    self._swap(self._get_value(char))
            else:
                if char not in self.dict:
                    self.dict[char] = previous_value

                self.result_list.append(char)

    #Performs the Run Length decoding
    def run_length_decode(self):
        zero_found = False
        for char in self.run_length_list:
            if (zero_found == True):
                for i in range(0, char):
                    self.mtf_list.append(1)
                zero_found = False
                continue
            
            if (isinstance(char, int)):
                if (char == 0):
                    zero_found = True
                else:
                    self.mtf_list.append(char)
            else:
                self.mtf_list.append(char)


 