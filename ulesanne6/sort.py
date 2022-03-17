import re


items = []

regex = "\([\d.\d+]+\)"
regex2 = "\([\d.\d+]+"

def sort_data(items, filename, withRules=False):
    with open(filename) as file:
        lines = file.readlines()
        for line in lines:
            if withRules:
                x = re.search(regex2, line)
            else:    
                x = re.search(regex, line)
            result = x.group(0).strip('(')
            if not withRules:
                result = result.strip(')')
            result = float(result)
            items.append((line, result))
    items.sort(reverse=True, key=lambda x: x[1])        


            
def write(items, filename):
    with open(filename, 'w') as f:
        for item in items:
            f.write("%s" % item[0])
            

sort_data(items, "output2_tr_2_percent.txt", True)
write(items, "sorted.txt")


