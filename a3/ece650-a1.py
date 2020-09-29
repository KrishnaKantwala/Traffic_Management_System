#!/usr/bin/env python

import sys
import re
import numpy as np

# YOUR CODE GOES HERE


def add(st, co, data_sheet):

    if len(data_sheet) == 0:
        data_sheet = dict(zip(st, co))
        return data_sheet
    else:
        for key in data_sheet.keys():
            if key == str(st[0]).lower():
                print("Error: Not allowed to add same street name more than one")
                return data_sheet

    data_sheet.update(dict(zip(st, co)))
    return data_sheet


def remove(st, data_sheet):
    if len(data_sheet) == 0:
        print("Error: No data to proceed")
        return data_sheet
    else:
        for key in data_sheet.keys():
            if key == str(st[0]).lower():
                del data_sheet[key]
                return data_sheet

    print("Error: Specified street name is not present")
    return data_sheet


def change(st, co, data_sheet):
    if len(data_sheet) == 0:
        print("Error: No data to proceed")
        return data_sheet
    else:
        for key in data_sheet.keys():
            if key == str(st[0]).lower():
                data_sheet.update(dict(zip(st, co)))
                return data_sheet

    print("Error: Specified street name is not present")
    return data_sheet


def generate_graph(data_sheet):

    list1 = []
    intersaction_list = []
    vertx_list = []
    edge_list = []
    for key in data_sheet:
        street_cor1 = data_sheet[key]
        list1.append(street_cor1)

    for i in range(0, len(list1)):
        if list1[i] == list1[-1]:
            break
        cord1 = list1[i]
        for k in range(i+1, len(list1)):
            cord2 = list1[k]
        # cord2 = list1[i + 1]
            for m in range(0, len(cord1)):
                if cord1[m] == cord1[-1]:
                    break
                cor1 = eval(cord1[m])
                cor2 = eval(cord1[m+1])
                for n in range(0, len(cord2)):
                    if cord2[n] == cord2[-1]:
                        break
                    cor3 = eval(cord2[n])
                    cor4 = eval(cord2[n + 1])
                    l1, l2 = calculate_intersection(cor1, cor2, cor3, cor4)
                    if l1 == "parallel":
                        pass
                    elif l1 == "nv":
                        pass
                    elif l1 == "overlap":
                        vertx_list.append(cor1)
                        vertx_list.append(cor2)
                        vertx_list.append(cor3)
                        vertx_list.append(cor4)
                        edge_list.append((cor1, cor3))
                        edge_list.append((cor3, cor2))
                        edge_list.append((cor2, cor4))
                    else:
                        intersaction_list.append(((cor1, cor2), (l1, l2)))
                        intersaction_list.append(((cor3, cor4), (l1, l2)))

    intersaction_list.sort()
    intersaction_list = list(set(intersaction_list))

    match_intersaction_list = []
    diff_intersaction_list = []
    for k in range(0, len(intersaction_list)):
        value1 = intersaction_list[k]
        for m in range(k + 1, len(intersaction_list)):
            value2 = intersaction_list[m]
            if value1[0] == value2[0] or value1[0][0] == value2[0][1] and value1[0][1] == value2[0][0]:
                sort_value1 = min(value1[0][0], value1[0][1]), max(value1[0][0], value1[0][1])
                sort_value2 = min(value2[0][0], value2[0][1]), max(value2[0][0], value2[0][1])
                match_intersaction_list.append((sort_value1, value1[1]))
                # print(sort_value1, value1[1])
                match_intersaction_list.append((sort_value2, value2[1]))
                # print(sort_value2, value2[1])

    match_intersaction_list = list(set(match_intersaction_list))
    label = 0
    if len(match_intersaction_list) == 0:
        diff_intersaction_list = intersaction_list.copy()
    else:
        for m in range(0, len(intersaction_list)):
            label = 0
            value3 = intersaction_list[m][0]
            for n in range(0, len(match_intersaction_list)):
                value4 = match_intersaction_list[n][0]
                if value3 == value4 or (value3[0] == value4[1] and value3[1] == value4[0]):
                    label = 1
            if label != 1:
                diff_intersaction_list.append((value3, intersaction_list[m][1]))

    list1 = list(set(match_intersaction_list))
    list2 = list(set(diff_intersaction_list))
    for a in range(0, len(list2)):
        if list2[a][0][0] != list2[a][1]:
            edge_list.append((list2[a][0][0], list2[a][1]))
        if list2[a][0][1] != list2[a][1]:
            edge_list.append((list2[a][0][1], list2[a][1]))
        vertx_list.append(list2[a][0][0])
        vertx_list.append(list2[a][0][1])
        vertx_list.append(list2[a][1])
    for b in range(0, len(list1)):
        val1 = list1[b][1]
        val3 = list1[b][0]
        for c in range(b + 1, len(list1)):
            val2 = list1[c][1]
            val4 = list1[c][0]
            if val3 == val4:
                vertx_list.append(val1)
                vertx_list.append(val2)
                vertx_list.append(val3[0])
                vertx_list.append(val3[1])

    final_match_list = []
    i = 0
    while i < len(list1):
        value1 = list1[i][0]
        final_value = []
        j = i + 1
        while j < len(list1):
            value2 = list1[j][0]
            if value1 == value2:
                final_value.append(value1[0])
                final_value.append(value1[1])
                final_value.append(list1[i][1])
                final_value.append(list1[j][1])
                list1.pop(j)
                j = j - 1
                # print(set(final_value))
                final_value = list(set(final_value))
                final_value.sort()
                # print(final_value)
            j = j + 1

        final_match_list.append(final_value)
        # print(final_match_list)
        i = i + 1
    # print(final_match_list)

    for i in range(0, len(final_match_list)):
        for j in range(0, len(final_match_list[i]) - 1):
            edge_list.append((final_match_list[i][j], final_match_list[i][j + 1]))

    final_vertex_list = set(vertx_list)
    final_edge_list = set(edge_list)
    print_graph(final_vertex_list, final_edge_list)


def print_graph(final_vertex_list, final_edge_list):

    list_one = []
    list_two = []

    for id, val in enumerate(final_vertex_list):
        # print("   ", id + 1, ":", val)
        list_one.append((id, val))

   #  print('V = {')
   # for id, val in enumerate(final_vertex_list):
   #     print("   ", (id + 1), ":", "(%.2f, %.2f)" % (val[0], val[1]))
   #  print('}')

    for value in final_edge_list:
        edge1 = value[0]
        edge2 = value[1]
        for item in list_one:
            if edge1 == item[1]:
                edge1 = item[0]
            if edge2 == item[1]:
                edge2 = item[0]
        list_two.append((edge1, edge2))

    print('V ' + str(len(list_one)))
    print('E {',end="")
    for value in list_two:
        if value == list_two[-1]:
            print("<" + str(value[0]) + "," + str(value[1]) + ">",end="")
        else:
            print("<" + str(value[0]) + "," + str(value[1]) + ">,",end="")
    print('}')
    sys.stdout.flush();


def calculate_intersection(a1, a2, b1, b2):

    vertical_stack = np.vstack([a1, a2, b1, b2])
    homo_cord = np.hstack((vertical_stack, np.ones((4, 1))))
    l1 = np.cross(homo_cord[0], homo_cord[1])
    l2 = np.cross(homo_cord[2], homo_cord[3])
    x, y, z = np.cross(l1, l2)  # point of intersection
    if z == 0:  # lines are parallel
        if x == 0 and y == 0:
            return "overlap", ""
        return "parallel", ""

    x_intersection = x / z
    y_intersection = y / z
    xa1 = a1[0]
    xa2 = a2[0]
    xa3 = a1[1]
    xa4 = a2[1]
    ya1 = b1[0]
    ya2 = b2[0]
    ya3 = b1[1]
    ya4 = b2[1]

    if max(min(xa1, xa2), min(ya1, ya2)) != min(max(xa1, xa2), max(ya1, ya2)):
        if ((x_intersection < max(min(xa1, xa2), min(ya1, ya2)))
                or (x_intersection > min(max(xa1, xa2), max(ya1, ya2)))):
            return "nv", ""
        else:
            return x_intersection, y_intersection
    elif max(min(xa3, xa4), min(ya3, ya4)) != min(max(xa3, xa4), max(ya3, ya4)):
        if ((y_intersection < max(min(xa3, xa4), min(ya3, ya4)))
                or (y_intersection > min(max(xa3, xa4), max(ya3, ya4)))):
            return "nv", ""
        else:
            return x_intersection, y_intersection
    else:
        return "nv", ""


def check_input(line):

    line_input = " ".join(line.split())
    user = re.split('\"|\s', line_input)
    cord_input = line_input.replace(" ", "")
    r = re.compile('[acrg]+')
    street_name_list = re.findall(r'"([^"]*)"', line_input)
    cordinates_list = re.findall(r"\(-?[0-9]+,-?[0-9]+\)", cord_input)
    street = []
    co_ordinates = []
    cordinates_len = len(cordinates_list)
    flag = 0
    user_input = ''
    if len(user[0]) > 1:
        print("Error: Wrong command")
        flag = 1
    else:
        user_input = line_input[0]
        if r.match(user_input) and user_input != 'g':
            # print("match with regex")
            if len(street_name_list) == 0:
                print("Error: Please enter street name and co_ordinates")
                flag = 1
            else:
                if len(street_name_list) > 1:
                    print("Error: more than one street name are not allowed")
                    flag = 1
                else:
                    street_name = street_name_list[0]
                    street.append(street_name.lower())
                    if cordinates_len == 0 and user_input != 'r':
                        print("Error :  Please provide co-ordinates for street")
                        flag = 1
                    elif user_input != 'r':
                        if cordinates_len < 2:
                            print("Error: Please provide valid co-ordinates")
                            flag = 1
                        else:
                            co_ordinates.append(cordinates_list)

        elif user_input == 'g':
            pass

        else:
            print("Error: Wrong command")
            flag = 1

    return street, co_ordinates, user_input, flag


def main():
    # YOUR MAIN CODE GOES HERE

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    data_sheet = {}
    while True:

        line = sys.stdin.readline()
        # print(line)
        if line == '':
            break
        # print('read a line:', user_input)
        st, co, user_input, f = check_input(line.strip())
        if f == 0:
            if user_input == 'a':
                data_sheet = add(st, co, data_sheet)
            elif user_input == 'r':
                remove(st, data_sheet)
            elif user_input == 'c':
                change(st, co, data_sheet)
            elif user_input == 'g':
                generate_graph(data_sheet)
            else:
                print("Error: Wrong command")

    # print('Finished reading input')
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()


