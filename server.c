#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include <windows.h>
#include <winhttp.h>
#include <json-c/json.h>

#define PORT 3000
#define BUFFER_SIZE 4096
#define JWT_SECRET "pharmacy-secret-key"

// Database connection
sqlite3 *db;

// Response structure
typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} response_t;

// Initialize response buffer
void init_response(response_t *resp) {
    resp->data = malloc(BUFFER_SIZE);
    resp->size = 0;
    resp->capacity = BUFFER_SIZE;
    resp->data[0] = '\0';
}

// Append to response buffer
void append_response(response_t *resp, const char *data) {
    size_t len = strlen(data);
    if (resp->size + len >= resp->capacity) {
        resp->capacity *= 2;
        resp->data = realloc(resp->data, resp->capacity);
    }
    strcat(resp->data, data);
    resp->size += len;
}

// Initialize database
int init_database() {
    int rc = sqlite3_open("pharmacy.db", &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    // Create tables
    const char *create_users = "CREATE TABLE IF NOT EXISTS users ("
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "full_name TEXT NOT NULL,"
        "email TEXT,"
        "phone TEXT,"
        "role TEXT DEFAULT 'user',"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    const char *create_categories = "CREATE TABLE IF NOT EXISTS categories ("
        "category_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "category_name TEXT UNIQUE NOT NULL,"
        "description TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    const char *create_suppliers = "CREATE TABLE IF NOT EXISTS suppliers ("
        "supplier_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "supplier_name TEXT NOT NULL,"
        "phone TEXT,"
        "email TEXT,"
        "address TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    const char *create_medicines = "CREATE TABLE IF NOT EXISTS medicines ("
        "medicine_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "medicine_name TEXT NOT NULL,"
        "generic_name TEXT,"
        "category_id INTEGER,"
        "supplier_id INTEGER,"
        "buying_price REAL,"
        "selling_price REAL,"
        "stock_quantity INTEGER DEFAULT 0,"
        "expiry_date TEXT,"
        "batch_number TEXT,"
        "rack_location TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (category_id) REFERENCES categories(category_id),"
        "FOREIGN KEY (supplier_id) REFERENCES suppliers(supplier_id)"
        ");";
    
    const char *create_patients = "CREATE TABLE IF NOT EXISTS patients ("
        "patient_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "patient_name TEXT NOT NULL,"
        "age INTEGER,"
        "gender TEXT,"
        "phone TEXT,"
        "email TEXT,"
        "address TEXT,"
        "medical_history TEXT,"
        "allergies TEXT,"
        "emergency_contact TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    const char *create_sales = "CREATE TABLE IF NOT EXISTS sales ("
        "sale_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "invoice_number TEXT UNIQUE NOT NULL,"
        "patient_id INTEGER,"
        "user_id INTEGER,"
        "sale_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "subtotal REAL DEFAULT 0,"
        "discount_amount REAL DEFAULT 0,"
        "tax_amount REAL DEFAULT 0,"
        "total_amount REAL DEFAULT 0,"
        "payment_method TEXT,"
        "payment_status TEXT DEFAULT 'pending',"
        "notes TEXT,"
        "FOREIGN KEY (patient_id) REFERENCES patients(patient_id),"
        "FOREIGN KEY (user_id) REFERENCES users(user_id)"
        ");";
    
    const char *create_sale_items = "CREATE TABLE IF NOT EXISTS sale_items ("
        "sale_item_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sale_id INTEGER,"
        "medicine_id INTEGER,"
        "quantity INTEGER,"
        "unit_price REAL,"
        "total_price REAL,"
        "batch_number TEXT,"
        "FOREIGN KEY (sale_id) REFERENCES sales(sale_id),"
        "FOREIGN KEY (medicine_id) REFERENCES medicines(medicine_id)"
        ");";
    
    // Execute table creation
    sqlite3_exec(db, create_users, NULL, NULL, NULL);
    sqlite3_exec(db, create_categories, NULL, NULL, NULL);
    sqlite3_exec(db, create_suppliers, NULL, NULL, NULL);
    sqlite3_exec(db, create_medicines, NULL, NULL, NULL);
    sqlite3_exec(db, create_patients, NULL, NULL, NULL);
    sqlite3_exec(db, create_sales, NULL, NULL, NULL);
    sqlite3_exec(db, create_sale_items, NULL, NULL, NULL);
    
    // Insert default admin user if not exists
    const char *check_admin = "SELECT COUNT(*) FROM users WHERE username = 'admin';";
    sqlite3_stmt *stmt;
    int admin_exists = 0;
    
    if (sqlite3_prepare_v2(db, check_admin, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            admin_exists = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    if (!admin_exists) {
        const char *insert_admin = "INSERT INTO users (username, password, full_name, role) "
            "VALUES ('admin', 'admin123', 'Administrator', 'Admin');";
        sqlite3_exec(db, insert_admin, NULL, NULL, NULL);
        
        const char *insert_pharmacist = "INSERT INTO users (username, password, full_name, role) "
            "VALUES ('pharmacist1', 'pharma123', 'John Pharmacist', 'Pharmacist');";
        sqlite3_exec(db, insert_pharmacist, NULL, NULL, NULL);
    }
    
    return 1;
}

// Generate JSON response
void send_json_response(response_t *resp, const char *status, const char *message, json_object *data) {
    json_object *json = json_object_new_object();
    json_object_object_add(json, "status", json_object_new_string(status));
    json_object_object_add(json, "message", json_object_new_string(message));
    
    if (data) {
        json_object_object_add(json, "data", data);
    }
    
    const char *json_string = json_object_to_json_string(json);
    append_response(resp, "HTTP/1.1 200 OK\r\n");
    append_response(resp, "Content-Type: application/json\r\n");
    append_response(resp, "Access-Control-Allow-Origin: *\r\n");
    append_response(resp, "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n");
    append_response(resp, "Access-Control-Allow-Headers: Content-Type, Authorization\r\n");
    append_response(resp, "\r\n");
    append_response(resp, json_string);
    
    json_object_put(json);
}

// Handle login endpoint
void handle_login(response_t *resp, const char *body) {
    // Parse JSON body
    json_object *json = json_tokener_parse(body);
    if (!json) {
        send_json_response(resp, "error", "Invalid JSON", NULL);
        return;
    }
    
    json_object *username_obj, *password_obj;
    if (!json_object_object_get_ex(json, "username", &username_obj) ||
        !json_object_object_get_ex(json, "password", &password_obj)) {
        send_json_response(resp, "error", "Missing username or password", NULL);
        json_object_put(json);
        return;
    }
    
    const char *username = json_object_get_string(username_obj);
    const char *password = json_object_get_string(password_obj);
    
    // Query database
    const char *query = "SELECT user_id, username, password, full_name, role FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *db_password = (const char*)sqlite3_column_text(stmt, 2);
            
            // Simple password check (in production, use bcrypt)
            if (strcmp(password, db_password) == 0) {
                // Create user data
                json_object *user_data = json_object_new_object();
                json_object_object_add(user_data, "id", json_object_new_int(sqlite3_column_int(stmt, 0)));
                json_object_object_add(user_data, "username", json_object_new_string((const char*)sqlite3_column_text(stmt, 1)));
                json_object_object_add(user_data, "name", json_object_new_string((const char*)sqlite3_column_text(stmt, 3)));
                json_object_object_add(user_data, "role", json_object_new_string((const char*)sqlite3_column_text(stmt, 4)));
                
                // Create token (simple implementation)
                char token[256];
                snprintf(token, sizeof(token), "token-%s-%ld", username, time(NULL));
                
                json_object *response_data = json_object_new_object();
                json_object_object_add(response_data, "token", json_object_new_string(token));
                json_object_object_add(response_data, "user", user_data);
                
                send_json_response(resp, "success", "Login successful", response_data);
                json_object_put(response_data);
            } else {
                send_json_response(resp, "error", "Invalid credentials", NULL);
            }
        } else {
            send_json_response(resp, "error", "User not found", NULL);
        }
        
        sqlite3_finalize(stmt);
    } else {
        send_json_response(resp, "error", "Database error", NULL);
    }
    
    json_object_put(json);
}

// Handle medicines endpoint
void handle_medicines(response_t *resp) {
    const char *query = "SELECT m.medicine_id, m.medicine_name, m.generic_name, "
        "c.category_name, s.supplier_name, m.stock_quantity, m.selling_price, "
        "m.expiry_date, m.batch_number, m.rack_location "
        "FROM medicines m "
        "LEFT JOIN categories c ON m.category_id = c.category_id "
        "LEFT JOIN suppliers s ON m.supplier_id = s.supplier_id "
        "ORDER BY m.medicine_name;";
    
    sqlite3_stmt *stmt;
    json_object *medicines_array = json_object_new_array();
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json_object *medicine = json_object_new_object();
            json_object_object_add(medicine, "medicine_id", json_object_new_int(sqlite3_column_int(stmt, 0)));
            json_object_object_add(medicine, "medicine_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 1)));
            json_object_object_add(medicine, "generic_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 2)));
            json_object_object_add(medicine, "category_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 3) ? (const char*)sqlite3_column_text(stmt, 3) : ""));
            json_object_object_add(medicine, "supplier_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 4) ? (const char*)sqlite3_column_text(stmt, 4) : ""));
            json_object_object_add(medicine, "stock_quantity", json_object_new_int(sqlite3_column_int(stmt, 5)));
            json_object_object_add(medicine, "selling_price", json_object_new_double(sqlite3_column_double(stmt, 6)));
            json_object_object_add(medicine, "expiry_date", json_object_new_string((const char*)sqlite3_column_text(stmt, 7) ? (const char*)sqlite3_column_text(stmt, 7) : ""));
            json_object_object_add(medicine, "batch_number", json_object_new_string((const char*)sqlite3_column_text(stmt, 8) ? (const char*)sqlite3_column_text(stmt, 8) : ""));
            json_object_object_add(medicine, "rack_location", json_object_new_string((const char*)sqlite3_column_text(stmt, 9) ? (const char*)sqlite3_column_text(stmt, 9) : ""));
            
            json_object_array_add(medicines_array, medicine);
        }
        sqlite3_finalize(stmt);
    }
    
    send_json_response(resp, "success", "Medicines retrieved", medicines_array);
    json_object_put(medicines_array);
}

// Handle patients endpoint
void handle_patients(response_t *resp) {
    const char *query = "SELECT patient_id, patient_name, age, gender, phone, email, address, "
        "medical_history, allergies, emergency_contact FROM patients ORDER BY patient_name;";
    
    sqlite3_stmt *stmt;
    json_object *patients_array = json_object_new_array();
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json_object *patient = json_object_new_object();
            json_object_object_add(patient, "patient_id", json_object_new_int(sqlite3_column_int(stmt, 0)));
            json_object_object_add(patient, "patient_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 1)));
            json_object_object_add(patient, "age", json_object_new_int(sqlite3_column_int(stmt, 2)));
            json_object_object_add(patient, "gender", json_object_new_string((const char*)sqlite3_column_text(stmt, 3) ? (const char*)sqlite3_column_text(stmt, 3) : ""));
            json_object_object_add(patient, "phone", json_object_new_string((const char*)sqlite3_column_text(stmt, 4) ? (const char*)sqlite3_column_text(stmt, 4) : ""));
            json_object_object_add(patient, "email", json_object_new_string((const char*)sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : ""));
            json_object_object_add(patient, "address", json_object_new_string((const char*)sqlite3_column_text(stmt, 6) ? (const char*)sqlite3_column_text(stmt, 6) : ""));
            json_object_object_add(patient, "medical_history", json_object_new_string((const char*)sqlite3_column_text(stmt, 7) ? (const char*)sqlite3_column_text(stmt, 7) : ""));
            json_object_object_add(patient, "allergies", json_object_new_string((const char*)sqlite3_column_text(stmt, 8) ? (const char*)sqlite3_column_text(stmt, 8) : ""));
            json_object_object_add(patient, "emergency_contact", json_object_new_string((const char*)sqlite3_column_text(stmt, 9) ? (const char*)sqlite3_column_text(stmt, 9) : ""));
            
            json_object_array_add(patients_array, patient);
        }
        sqlite3_finalize(stmt);
    }
    
    send_json_response(resp, "success", "Patients retrieved", patients_array);
    json_object_put(patients_array);
}

// Handle categories endpoint
void handle_categories(response_t *resp) {
    const char *query = "SELECT category_id, category_name, description FROM categories ORDER BY category_name;";
    
    sqlite3_stmt *stmt;
    json_object *categories_array = json_object_new_array();
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json_object *category = json_object_new_object();
            json_object_object_add(category, "category_id", json_object_new_int(sqlite3_column_int(stmt, 0)));
            json_object_object_add(category, "category_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 1)));
            json_object_object_add(category, "description", json_object_new_string((const char*)sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : ""));
            
            json_object_array_add(categories_array, category);
        }
        sqlite3_finalize(stmt);
    }
    
    send_json_response(resp, "success", "Categories retrieved", categories_array);
    json_object_put(categories_array);
}

// Handle suppliers endpoint
void handle_suppliers(response_t *resp) {
    const char *query = "SELECT supplier_id, supplier_name, phone, email, address FROM suppliers ORDER BY supplier_name;";
    
    sqlite3_stmt *stmt;
    json_object *suppliers_array = json_object_new_array();
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json_object *supplier = json_object_new_object();
            json_object_object_add(supplier, "supplier_id", json_object_new_int(sqlite3_column_int(stmt, 0)));
            json_object_object_add(supplier, "supplier_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 1)));
            json_object_object_add(supplier, "phone", json_object_new_string((const char*)sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : ""));
            json_object_object_add(supplier, "email", json_object_new_string((const char*)sqlite3_column_text(stmt, 3) ? (const char*)sqlite3_column_text(stmt, 3) : ""));
            json_object_object_add(supplier, "address", json_object_new_string((const char*)sqlite3_column_text(stmt, 4) ? (const char*)sqlite3_column_text(stmt, 4) : ""));
            
            json_object_array_add(suppliers_array, supplier);
        }
        sqlite3_finalize(stmt);
    }
    
    send_json_response(resp, "success", "Suppliers retrieved", suppliers_array);
    json_object_put(suppliers_array);
}

// Handle sales endpoint
void handle_sales(response_t *resp) {
    const char *query = "SELECT s.sale_id, s.invoice_number, p.patient_name, s.sale_date, "
        "s.total_amount, s.payment_method, s.payment_status "
        "FROM sales s "
        "LEFT JOIN patients p ON s.patient_id = p.patient_id "
        "ORDER BY s.sale_date DESC;";
    
    sqlite3_stmt *stmt;
    json_object *sales_array = json_object_new_array();
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json_object *sale = json_object_new_object();
            json_object_object_add(sale, "sale_id", json_object_new_int(sqlite3_column_int(stmt, 0)));
            json_object_object_add(sale, "invoice_number", json_object_new_string((const char*)sqlite3_column_text(stmt, 1)));
            json_object_object_add(sale, "patient_name", json_object_new_string((const char*)sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : ""));
            json_object_object_add(sale, "sale_date", json_object_new_string((const char*)sqlite3_column_text(stmt, 3)));
            json_object_object_add(sale, "total_amount", json_object_new_double(sqlite3_column_double(stmt, 4)));
            json_object_object_add(sale, "payment_method", json_object_new_string((const char*)sqlite3_column_text(stmt, 5) ? (const char*)sqlite3_column_text(stmt, 5) : ""));
            json_object_object_add(sale, "payment_status", json_object_new_string((const char*)sqlite3_column_text(stmt, 6) ? (const char*)sqlite3_column_text(stmt, 6) : ""));
            
            json_object_array_add(sales_array, sale);
        }
        sqlite3_finalize(stmt);
    }
    
    send_json_response(resp, "success", "Sales retrieved", sales_array);
    json_object_put(sales_array);
}

// Simple HTTP server using Windows Sockets
void start_server() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return;
    }
    
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return;
    }
    
    // Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    
    // Listen for connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    
    printf("Pharmacy Management System running on http://localhost:%d\n", PORT);
    printf("Database: pharmacy.db (SQLite)\n");
    printf("Default credentials:\n");
    printf("  Admin: admin/admin123\n");
    printf("  Pharmacist: pharmacist1/pharma123\n");
    
    // Main server loop
    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }
        
        // Receive request
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            // Parse HTTP request
            char method[16], path[256];
            sscanf(buffer, "%s %s", method, path);
            
            response_t resp;
            init_response(&resp);
            
            // Route handling
            if (strcmp(method, "POST") == 0 && strcmp(path, "/api/login") == 0) {
                // Extract body
                char *body = strstr(buffer, "\r\n\r\n");
                if (body) {
                    body += 4;
                    handle_login(&resp, body);
                } else {
                    send_json_response(&resp, "error", "No body found", NULL);
                }
            } else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/medicines") == 0) {
                handle_medicines(&resp);
            } else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/patients") == 0) {
                handle_patients(&resp);
            } else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/categories") == 0) {
                handle_categories(&resp);
            } else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/suppliers") == 0) {
                handle_suppliers(&resp);
            } else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/sales") == 0) {
                handle_sales(&resp);
            } else if (strcmp(method, "OPTIONS") == 0) {
                append_response(&resp, "HTTP/1.1 200 OK\r\n");
                append_response(&resp, "Access-Control-Allow-Origin: *\r\n");
                append_response(&resp, "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n");
                append_response(&resp, "Access-Control-Allow-Headers: Content-Type, Authorization\r\n");
                append_response(&resp, "\r\n");
            } else {
                append_response(&resp, "HTTP/1.1 404 Not Found\r\n");
                append_response(&resp, "Content-Type: application/json\r\n");
                append_response(&resp, "\r\n");
                append_response(&resp, "{\"status\":\"error\",\"message\":\"Endpoint not found\"}");
            }
            
            // Send response
            send(clientSocket, resp.data, strlen(resp.data), 0);
            
            free(resp.data);
        }
        
        closesocket(clientSocket);
    }
    
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    printf("Starting Pharmacy Management System (C Backend)\n");
    printf("===============================================\n");
    
    // Initialize database
    if (!init_database()) {
        printf("Failed to initialize database\n");
        return 1;
    }
    
    printf("Database initialized successfully\n");
    
    // Start server
    start_server();
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}
