import csv


output = []

def make_lowercase(msg1):
    msg2 = ''
    for i in msg1:
        if ord(i) == 213:
            i = "õ"
            print(msg1)
        elif ord(i) == 352:
            i = "š"
        elif ord(i) >= 65 and ord(i) <= 90:
            i = chr(ord(i)+32)
        msg2 = msg2 + i
    return msg2    

def format_string(string):
    lowercase = make_lowercase(string)
    return lowercase.replace(" ", "")


with open('tshekid.csv', encoding='utf-8') as csvfile:
    reader = csv.reader(csvfile)
    next(reader) # skip line
    previous_customer_id = 1
    i = 0
    for row in reader:
        customer_id = int(row[0])
        item = format_string(row[1])
        if customer_id == previous_customer_id:
            if len(output) < (i + 1):
                output.append(str())
                output[i] = output[i] + item
            else: 
                output[i] = output[i] + " " + item
        else:
            i += 1
            if len(output) < (i + 1):
                output.append(str())
                output[i] = output[i] + item
            else: 
                output[i] = output[i] + " " + item
        previous_customer_id = customer_id        


with open('output2.txt', 'w') as f:
    for item in output:
        f.write("%s\n" % item)
