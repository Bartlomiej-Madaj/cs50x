from cs50 import SQL

db = SQL("sqlite:///roster.db")

students_table = db.execute("CREATE TABLE students (id INTEGER NOT NULL,student_name TEXT,houses_id INTEGER, PRIMARY KEY(id), FOREIGN KEY (houses_id) REFERENCES houses(id))")

houses_table = db.execute("CREATE TABLE houses (id INTEGER NOT NULL,house_name TEXT, PRIMARY KEY(id)")

