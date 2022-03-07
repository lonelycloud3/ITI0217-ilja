import csv
import mysql.connector as msql
from mysql.connector import Error

def create_tables(cursor):
    try:
        cursor.execute("CREATE TABLE Block (id INT PRIMARY KEY, block_hash VARCHAR(10000) NOT NULL, block_time INT UNSIGNED NOT NULL);")
        cursor.execute("""CREATE TABLE Transaction (id INT PRIMARY KEY, hash VARCHAR(10000) NOT NULL, blockID INT NOT NULL,
    CONSTRAINT fk_blockID FOREIGN KEY (blockID) REFERENCES Block(id));""")
        cursor.execute("""CREATE TABLE Output (id INT PRIMARY KEY, dstAddress VARCHAR(10000) NOT NULL, value BIGINT NOT NULL, txID INT NOT NULL, offset INT NOT NULL, 
    CONSTRAINT fk_txID FOREIGN KEY (txID) REFERENCES Transaction(id));""")
        cursor.execute("""CREATE TABLE Input (id INT PRIMARY KEY, outputID INT NOT NULL, txID INT NOT NULL, offset INT NOT NULL, 
    CONSTRAINT fk_outputID FOREIGN KEY (outputID) REFERENCES Output(id), 
    CONSTRAINT fk_input_txID FOREIGN KEY (txID) REFERENCES Transaction(id));""")
    except Error as e:
        print("Error while creating tables", e)
            
def insert_data(cursor, conn, filename, command, getdata):
    with open(filename) as csvfile:
        reader = csv.reader(csvfile, delimiter=';')
        next(reader) # skip header
        for row in reader:
            data = getdata(row)
            cursor.execute(command, data)
    conn.commit()    
        

def print_result(cursor, result):
    field_names = [i[0] for i in cursor.description]
    print(field_names)
    for row in result:
        print(row)

# queries
def make_query(cursor, conn, command):
    cursor.execute(command)
    result = cursor.fetchall()
    print_result(cursor, result)

def insert_into_command(X, command):
    return command.replace('X',X)


if __name__ == '__main__':
    conn = msql.connect(host='localhost', user='root',  
                            password='132456')#give ur username, password
    if conn.is_connected():
        cursor = conn.cursor()
    else:
        print("Error while connecting to MySQL")
    cursor.execute("USE bitcoin;")
    #create_tables(cursor)

    #insert_data(cursor, conn, "blocks.csv", "INSERT INTO Block VALUES (%s, %s, %s);", lambda row: (int(row[0]), row[1], int(row[2])))
    #insert_data(cursor, conn, "transactions.csv", "INSERT INTO Transaction VALUES (%s, %s, %s);", lambda row: (int(row[0]), row[1], int(row[2])))
    #insert_data(cursor, conn, "outputs.csv", "INSERT INTO Output VALUES (%s, %s, %s, %s, %s);", lambda row: (int(row[0]), row[1], int(row[2]), int(row[3]), int(row[4])))
    #insert_data(cursor, conn, "inputs.csv", "INSERT INTO Input VALUES (%s, %s, %s, %s);", lambda row: (int(row[0]), int(row[1]), int(row[2]), int(row[3])))

    #1
    #make_query(cursor, conn, "SELECT *, (SELECT Count(*) FROM Transaction WHERE Transaction.blockID = Block.id) AS TransactionCount FROM Block ORDER BY TransactionCount DESC LIMIT 10;")
    #3
    #make_query(cursor, conn, "SELECT *, (SELECT Count(*) FROM Input WHERE Transaction.id = Input.txID) AS InputCount FROM Transaction ORDER BY InputCount DESC LIMIT 10;")
    #4
    #make_query(cursor, conn, "SELECT *, (SELECT Count(*) FROM Output WHERE Transaction.id = Output.txID) AS OutputCount FROM Transaction ORDER BY OutputCount DESC LIMIT 10;")
    #6
    #make_query(cursor, conn, insert_into_command("504","SELECT *, (SELECT Sum(value) FROM Output WHERE id IN (SELECT outputID FROM Input WHERE txID=X)) as Input_Sum FROM Transaction WHERE id=X;"))
    #7
    #make_query(cursor, conn, insert_into_command("504","SELECT *, (SELECT Sum(value) FROM Output WHERE txID=X) as Output_Sum FROM Transaction WHERE id=X;"))
    #8
    #make_query(cursor, conn, insert_into_command("504", "SELECT dstAddress FROM Output WHERE id IN (SELECT outputID FROM Input WHERE txID=X);"))
    #9
    #make_query(cursor, conn, insert_into_command("504", "SELECT dstAddress FROM Output WHERE txID=X"))
    #11
    #make_query(cursor, conn, "SELECT dstAddress, Count(*) as Input_Count FROM Output WHERE id IN (SELECT outputID FROM Input) GROUP BY dstAddress ORDER BY Input_Count DESC LIMIT 10;")
    #12
    #make_query(cursor, conn, "SELECT dstAddress, Count(*) AS Output_Count FROM Output GROUP BY dstAddress ORDER BY Output_Count DESC LIMIT 10;")
    #13
    #make_query(cursor, conn, "SELECT Block.id, Block.block_hash, Block.block_time, Sum(value) AS Total_Value FROM Block INNER JOIN (SELECT blockID, value FROM Transaction INNER JOIN Output ON Transaction.id = Output.txID) AS Temp ON Block.id = Temp.blockID GROUP BY blockID ORDER BY Total_Value DESC LIMIT 10;")
    
    
    

