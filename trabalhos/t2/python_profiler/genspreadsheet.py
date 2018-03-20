#!/usr/bin/python

# Spreadsheet generator (excel format) for uploading to the cloud
# Max. 50,000 characters/cell and 2,000,000 cells/spreadsheet
# Authors: fmarin@inf.ufsm.br
# Revised by: andrea@inf.ufsm.br

import sys
import openpyxl

def generate_sheet(character, n_characters, n_columns, n_rows):
    cell = ""
    row =  []
    spreadsheet = []
    for i in range(0, n_characters):
        cell += character
    for i in range(0, n_columns):
        row.append(cell)
    for i in range(0, n_rows):
        spreadsheet.append(row)
    return spreadsheet

def generate_xlsx(spreadsheet, file):
    wb = openpyxl.Workbook(write_only=True)
    ws = wb.create_sheet()
    for row in spreadsheet:
        ws.append(row)
    wb.save(file)

def main(argv):
    # fill char
    character = 'A'
    # set up sizes
    n_rows = 1000 if len(argv) < 2 else int(argv[1])
    n_columns = 10 if len(argv) < 3 else int(argv[2])
    n_characters = 100 if len(argv) < 4 else int(argv[3])
    
    # output csv file
    xlsx_file = ('iceis2018'
                '-row' + str(n_rows) +
                '-col' + str(n_columns) +
                '-cell' + str(n_characters) +
                '.xlsx')
    # generate spreadsheet
    spreadsheet = generate_sheet(character, n_characters, n_columns, n_rows)
    generate_xlsx(spreadsheet, xlsx_file)
    print('Success!')



if __name__ == "__main__":
    main(sys.argv)