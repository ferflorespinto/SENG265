# Name: Jorge Fernando Flores Pinto                                           #
# ID: V00880059                                                               #
# SENG265, Summer 2017                                                        #
#                                                                             #
# The class Encoding.py performs the encoding step for phase2.py. It takes    #
# a .ph1 file as input and does all necessary steps to output an encoded      #
# .ph2 file.                                                                  #
#                                                                             #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

import struct

class Encoding:
    def __init__(self, initial_string):
        self.initial_string = initial_string
        self.dict = {}
        self.mtf_list = []
        self.run_length_list = []

    #Moves a character to the first position of the list and shifts all characters
    #before it by one
    def _swap(self, key_to_swap):
        if (self.dict[key_to_swap] != 1):
            for key in self.dict:
                if (self.dict[key] < len(self.dict) and self.dict[key] < self.dict[key_to_swap]):
                    self.dict[key] += 1

            self.dict[key_to_swap] = 1

    #Performs the Move to Front encoding
    def mtf_encode(self):
        count_unique = 0
        for char in self.initial_string:
            if char not in self.dict:
                count_unique += 1
                self.dict[char] = count_unique
                self.mtf_list.append(self.dict[char])
                self.mtf_list.append(char)
                
            else:
                self.mtf_list.append(self.dict[char])
                self._swap(char)

    #Performs the Run Length encoding
    def run_length_encode(self):
        one_counter = 0
        next_elem = None
        list_size = len(self.mtf_list)
        loop_counter = 0
        overflow = 127
        diff = 0

        #Zero replacement is only done for strings of ones that are of length 3 or greater
        for element in self.mtf_list:
            loop_counter += 1
            if (loop_counter < list_size):
                next_elem = self.mtf_list[loop_counter]
           
            if (element == 1 and next_elem == 1):
                one_counter += 1
                if (loop_counter == list_size):
                    for i in range(0, one_counter):
                        self.run_length_list.append(element)

            else:
                one_counter += 1
                if(one_counter < 3):
                    for i in range(0, one_counter):
                        self.run_length_list.append(element)
               
                elif (one_counter >= 128):
                    diff = overflow
                    while(diff >= 127):
                        diff = one_counter - overflow
                        self.run_length_list.append(0)
                        self.run_length_list.append(overflow)
                    
                    if (diff < 3):
                        for i in range(0, diff):
                            self.run_length_list.append(1)

                    else:     
                        self.run_length_list.append(0)
                        self.run_length_list.append(diff)

                else:
                    self.run_length_list.append(0)
                    self.run_length_list.append(one_counter)

                one_counter = 0


