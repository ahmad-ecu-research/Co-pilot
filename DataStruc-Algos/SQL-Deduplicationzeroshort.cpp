#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string> // Add missing include directive for the <string> header file

//function to delete duplicate Emails and organise EMAILS with min ID lentgh
void deleteDuplicateEmails(sqlite3 *db){
    std::cout << "Deleting duplicate emails...\n";
    sqlite3_stmt *stmt;
    std::string sql = "DELETE FROM EMAILS WHERE ID NOT IN (SELECT MIN(ID) FROM EMAILS GROUP BY EMAIL)";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Duplicate emails deleted.\n";
    std::cout << "Organising emails...\n";
    sql = "UPDATE EMAILS SET ID = (SELECT COUNT(*) FROM EMAILS WHERE ID < t.ID AND EMAIL = t.EMAIL) + 1";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Emails organised.\n";
    std::cout << "Deleting zero length emails...\n";
    sql = "DELETE FROM EMAILS WHERE LENGTH(EMAIL) = 0";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Zero length emails deleted.\n";
    std::cout << "Deleting zero length emails...\n";
    sql = "DELETE FROM EMAILS WHERE LENGTH(EMAIL) = 0";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Zero length emails deleted.\n";

}
int main()
{
// create SQLite connection 
    sqlite3 *db;
    sqlite3_open("Emails.db", &db); // Correct the typo in the SQLite database file name
    sqlite3_stmt *stmt;
    std::string sql = "SELECT * FROM EMAILS";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    int rc = sqlite3_step(stmt);
    while (rc != SQLITE_DONE) {
        std::cout << sqlite3_column_int(stmt, 0) << " " << sqlite3_column_text(stmt, 1) << "\n";
        rc = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    deleteDuplicateEmails(db);
    sql = "SELECT * FROM EMAILS";  
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc != SQLITE_DONE) {
        std::cout << sqlite3_column_int(stmt, 0) << " " << sqlite3_column_text(stmt, 1) << "\n";
        rc = sqlite3_step(stmt);
    }
    // create table for emails 
    sql = "CREATE TABLE EMAILS (ID INTEGER PRIMARY KEY, EMAIL TEXT NOT NULL)";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

// read input file and insert emails and ID into table
    std::ifstream file("emails.txt");
    std::string email;
    int id = 1;
    while (std::getline(file, email)) {
        sql = "INSERT INTO EMAILS (ID, EMAIL) VALUES (" + std::to_string(id) + ", '" + email + "')";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        id++;
    }
    file.close();
    deleteDuplicateEmails(db);
    sql = "SELECT * FROM EMAILS";  
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc != SQLITE_DONE) {
        std::cout << sqlite3_column_int(stmt, 0) << " " << sqlite3_column_text(stmt, 1) << "\n";
        rc = sqlite3_step(stmt);
    }
    
    // write Eamils records to output file 
    std::ofstream output("output.txt");
    sql = "SELECT * FROM EMAILS";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc != SQLITE_DONE) {
        output << sqlite3_column_text(stmt, 1) << "\n";
        rc = sqlite3_step(stmt);
    }
    output.close();

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;

}
