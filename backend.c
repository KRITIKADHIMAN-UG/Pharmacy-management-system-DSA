/*
 * Pharmacy Management System - Backend (C Language)
 * Data structures and core functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Data Structures
typedef struct {
    int id;
    char username[50];
    char password[50];
    char role[20];
    char first_name[50];
    char last_name[50];
    char phone[20];
    int is_active;
} User;

typedef struct {
    int id;
    char first_name[50];
    char last_name[50];
    int age;
    char gender[10];
    char phone[20];
    char email[100];
    char medical_history[200];
    char allergies[100];
    int is_active;
} Patient;

typedef struct {
    int id;
    char first_name[50];
    char last_name[50];
    char specialization[100];
    char phone[20];
    char email[100];
    char shift[20];
    char license_number[50];
    int is_available;
} Doctor;

typedef struct {
    int id;
    char bed_number[20];
    char bed_type[20];
    char status[20];
    char ward[50];
    int patient_id;
    char assigned_at[50];
} Bed;

typedef struct {
    int id;
    char name[100];
    char expiry_date[20];
    int quantity;
    float unit_price;
    char category[50];
    int minimum_stock_level;
    int is_low_stock;
    int is_expired;
} Medicine;

// Global arrays to store data
User users[100];
Patient patients[100];
Doctor doctors[100];
Bed beds[100];
Medicine medicines[100];

int user_count = 0;
int patient_count = 0;
int doctor_count = 0;
int bed_count = 0;
int medicine_count = 0;

// Function prototypes
void initialize_data();
void save_data_to_file();
void load_data_from_file();
int authenticate_user(char* username, char* password);
void add_patient(char* first_name, char* last_name, int age, char* gender, char* phone, char* email, char* medical_history, char* allergies);
void add_doctor(char* first_name, char* last_name, char* specialization, char* phone, char* email, char* shift, char* license_number);
void add_bed(char* bed_number, char* bed_type, char* ward);
void add_medicine(char* name, char* expiry_date, int quantity, float unit_price, char* category, int minimum_stock_level);
void assign_bed(int bed_id, int patient_id);
void release_bed(int bed_id);
void get_dashboard_stats();
void generate_html_response(char* response);

// Initialize sample data
void initialize_data() {
    // Add sample users
    strcpy(users[0].username, "admin");
    strcpy(users[0].password, "admin123");
    strcpy(users[0].role, "admin");
    strcpy(users[0].first_name, "Admin");
    strcpy(users[0].last_name, "User");
    strcpy(users[0].phone, "555-0001");
    users[0].is_active = 1;
    users[0].id = 1;
    
    strcpy(users[1].username, "doctor");
    strcpy(users[1].password, "doctor123");
    strcpy(users[1].role, "doctor");
    strcpy(users[1].first_name, "Dr. John");
    strcpy(users[1].last_name, "Smith");
    strcpy(users[1].phone, "555-0002");
    users[1].is_active = 1;
    users[1].id = 2;
    
    strcpy(users[2].username, "patient");
    strcpy(users[2].password, "patient123");
    strcpy(users[2].role, "patient");
    strcpy(users[2].first_name, "Jane");
    strcpy(users[2].last_name, "Doe");
    strcpy(users[2].phone, "555-0003");
    users[2].is_active = 1;
    users[2].id = 3;
    
    user_count = 3;
    
    // Add sample patients
    strcpy(patients[0].first_name, "Alice");
    strcpy(patients[0].last_name, "Johnson");
    patients[0].age = 35;
    strcpy(patients[0].gender, "Female");
    strcpy(patients[0].phone, "123-456-7890");
    strcpy(patients[0].email, "alice@email.com");
    strcpy(patients[0].medical_history, "Diabetes, Hypertension");
    strcpy(patients[0].allergies, "Penicillin");
    patients[0].is_active = 1;
    patients[0].id = 1;
    
    strcpy(patients[1].first_name, "Bob");
    strcpy(patients[1].last_name, "Wilson");
    patients[1].age = 42;
    strcpy(patients[1].gender, "Male");
    strcpy(patients[1].phone, "123-456-7891");
    strcpy(patients[1].email, "bob@email.com");
    strcpy(patients[1].medical_history, "Asthma");
    strcpy(patients[1].allergies, "None");
    patients[1].is_active = 1;
    patients[1].id = 2;
    
    patient_count = 2;
    
    // Add sample doctors
    strcpy(doctors[0].first_name, "Dr. Sarah");
    strcpy(doctors[0].last_name, "Brown");
    strcpy(doctors[0].specialization, "Cardiology");
    strcpy(doctors[0].phone, "123-456-7801");
    strcpy(doctors[0].email, "sarah@hospital.com");
    strcpy(doctors[0].shift, "Morning");
    strcpy(doctors[0].license_number, "DOC001");
    doctors[0].is_available = 1;
    doctors[0].id = 1;
    
    strcpy(doctors[1].first_name, "Dr. Michael");
    strcpy(doctors[1].last_name, "Davis");
    strcpy(doctors[1].specialization, "Neurology");
    strcpy(doctors[1].phone, "123-456-7802");
    strcpy(doctors[1].email, "michael@hospital.com");
    strcpy(doctors[1].shift, "Evening");
    strcpy(doctors[1].license_number, "DOC002");
    doctors[1].is_available = 1;
    doctors[1].id = 2;
    
    doctor_count = 2;
    
    // Add sample beds
    strcpy(beds[0].bed_number, "G001");
    strcpy(beds[0].bed_type, "general");
    strcpy(beds[0].status, "available");
    strcpy(beds[0].ward, "Ward A");
    beds[0].patient_id = 0;
    strcpy(beds[0].assigned_at, "");
    beds[0].id = 1;
    
    strcpy(beds[1].bed_number, "ICU001");
    strcpy(beds[1].bed_type, "icu");
    strcpy(beds[1].status, "available");
    strcpy(beds[1].ward, "ICU");
    beds[1].patient_id = 0;
    strcpy(beds[1].assigned_at, "");
    beds[1].id = 2;
    
    strcpy(beds[2].bed_number, "ER001");
    strcpy(beds[2].bed_type, "emergency");
    strcpy(beds[2].status, "occupied");
    strcpy(beds[2].ward, "Emergency");
    beds[2].patient_id = 1;
    strcpy(beds[2].assigned_at, "2024-01-15 10:30:00");
    beds[2].id = 3;
    
    bed_count = 3;
    
    // Add sample medicines
    strcpy(medicines[0].name, "Paracetamol");
    strcpy(medicines[0].expiry_date, "2025-12-31");
    medicines[0].quantity = 100;
    medicines[0].unit_price = 5.50;
    strcpy(medicines[0].category, "Painkiller");
    medicines[0].minimum_stock_level = 20;
    medicines[0].is_low_stock = 0;
    medicines[0].is_expired = 0;
    medicines[0].id = 1;
    
    strcpy(medicines[1].name, "Amoxicillin");
    strcpy(medicines[1].expiry_date, "2024-06-30");
    medicines[1].quantity = 5;
    medicines[1].unit_price = 15.75;
    strcpy(medicines[1].category, "Antibiotic");
    medicines[1].minimum_stock_level = 10;
    medicines[1].is_low_stock = 1;
    medicines[1].is_expired = 0;
    medicines[1].id = 2;
    
    medicine_count = 2;
}

// Authentication function
int authenticate_user(char* username, char* password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0 && 
            users[i].is_active) {
            return users[i].id;
        }
    }
    return -1;
}

// Add new patient
void add_patient(char* first_name, char* last_name, int age, char* gender, char* phone, char* email, char* medical_history, char* allergies) {
    if (patient_count < 100) {
        strcpy(patients[patient_count].first_name, first_name);
        strcpy(patients[patient_count].last_name, last_name);
        patients[patient_count].age = age;
        strcpy(patients[patient_count].gender, gender);
        strcpy(patients[patient_count].phone, phone);
        strcpy(patients[patient_count].email, email);
        strcpy(patients[patient_count].medical_history, medical_history);
        strcpy(patients[patient_count].allergies, allergies);
        patients[patient_count].is_active = 1;
        patients[patient_count].id = patient_count + 1;
        patient_count++;
    }
}

// Add new doctor
void add_doctor(char* first_name, char* last_name, char* specialization, char* phone, char* email, char* shift, char* license_number) {
    if (doctor_count < 100) {
        strcpy(doctors[doctor_count].first_name, first_name);
        strcpy(doctors[doctor_count].last_name, last_name);
        strcpy(doctors[doctor_count].specialization, specialization);
        strcpy(doctors[doctor_count].phone, phone);
        strcpy(doctors[doctor_count].email, email);
        strcpy(doctors[doctor_count].shift, shift);
        strcpy(doctors[doctor_count].license_number, license_number);
        doctors[doctor_count].is_available = 1;
        doctors[doctor_count].id = doctor_count + 1;
        doctor_count++;
    }
}

// Add new bed
void add_bed(char* bed_number, char* bed_type, char* ward) {
    if (bed_count < 100) {
        strcpy(beds[bed_count].bed_number, bed_number);
        strcpy(beds[bed_count].bed_type, bed_type);
        strcpy(beds[bed_count].status, "available");
        strcpy(beds[bed_count].ward, ward);
        beds[bed_count].patient_id = 0;
        strcpy(beds[bed_count].assigned_at, "");
        beds[bed_count].id = bed_count + 1;
        bed_count++;
    }
}

// Add new medicine
void add_medicine(char* name, char* expiry_date, int quantity, float unit_price, char* category, int minimum_stock_level) {
    if (medicine_count < 100) {
        strcpy(medicines[medicine_count].name, name);
        strcpy(medicines[medicine_count].expiry_date, expiry_date);
        medicines[medicine_count].quantity = quantity;
        medicines[medicine_count].unit_price = unit_price;
        strcpy(medicines[medicine_count].category, category);
        medicines[medicine_count].minimum_stock_level = minimum_stock_level;
        medicines[medicine_count].is_low_stock = (quantity <= minimum_stock_level) ? 1 : 0;
        medicines[medicine_count].is_expired = 0; // Simplified for demo
        medicines[medicine_count].id = medicine_count + 1;
        medicine_count++;
    }
}

// Assign bed to patient
void assign_bed(int bed_id, int patient_id) {
    for (int i = 0; i < bed_count; i++) {
        if (beds[i].id == bed_id && strcmp(beds[i].status, "available") == 0) {
            strcpy(beds[i].status, "occupied");
            beds[i].patient_id = patient_id;
            time_t now = time(0);
            strftime(beds[i].assigned_at, sizeof(beds[i].assigned_at), "%Y-%m-%d %H:%M:%S", localtime(&now));
            break;
        }
    }
}

// Release bed
void release_bed(int bed_id) {
    for (int i = 0; i < bed_count; i++) {
        if (beds[i].id == bed_id && strcmp(beds[i].status, "occupied") == 0) {
            strcpy(beds[i].status, "available");
            beds[i].patient_id = 0;
            strcpy(beds[i].assigned_at, "");
            break;
        }
    }
}

// Get dashboard statistics
void get_dashboard_stats() {
    int total_patients = 0;
    int total_doctors = doctor_count;
    int total_beds = bed_count;
    int available_beds = 0;
    int occupied_beds = 0;
    int total_medicines = medicine_count;
    int low_stock_medicines = 0;
    int expired_medicines = 0;
    
    // Count active patients
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].is_active) total_patients++;
    }
    
    // Count bed status
    for (int i = 0; i < bed_count; i++) {
        if (strcmp(beds[i].status, "available") == 0) available_beds++;
        else if (strcmp(beds[i].status, "occupied") == 0) occupied_beds++;
    }
    
    // Count medicine issues
    for (int i = 0; i < medicine_count; i++) {
        if (medicines[i].is_low_stock) low_stock_medicines++;
        if (medicines[i].is_expired) expired_medicines++;
    }
    
    printf("{\n");
    printf("  \"total_patients\": %d,\n", total_patients);
    printf("  \"total_doctors\": %d,\n", total_doctors);
    printf("  \"total_beds\": %d,\n", total_beds);
    printf("  \"available_beds\": %d,\n", available_beds);
    printf("  \"occupied_beds\": %d,\n", occupied_beds);
    printf("  \"total_medicines\": %d,\n", total_medicines);
    printf("  \"low_stock_medicines\": %d,\n", low_stock_medicines);
    printf("  \"expired_medicines\": %d\n", expired_medicines);
    printf("}\n");
}

// Generate HTML response for web interface
void generate_html_response(char* response) {
    printf("Content-Type: text/html\n\n");
    printf("<!DOCTYPE html>\n");
    printf("<html><head><title>Pharmacy Management System</title></head><body>\n");
    printf("<h1>Pharmacy Management System - C Backend</h1>\n");
    printf("<p>Response: %s</p>\n", response);
    printf("</body></html>\n");
}

// Save data to file (simplified)
void save_data_to_file() {
    FILE *file = fopen("pharmacy_data.txt", "w");
    if (file != NULL) {
        fprintf(file, "Users: %d\n", user_count);
        fprintf(file, "Patients: %d\n", patient_count);
        fprintf(file, "Doctors: %d\n", doctor_count);
        fprintf(file, "Beds: %d\n", bed_count);
        fprintf(file, "Medicines: %d\n", medicine_count);
        fclose(file);
    }
}

// Load data from file (simplified)
void load_data_from_file() {
    FILE *file = fopen("pharmacy_data.txt", "r");
    if (file != NULL) {
        fscanf(file, "Users: %d\n", &user_count);
        fscanf(file, "Patients: %d\n", &patient_count);
        fscanf(file, "Doctors: %d\n", &doctor_count);
        fscanf(file, "Beds: %d\n", &bed_count);
        fscanf(file, "Medicines: %d\n", &medicine_count);
        fclose(file);
    } else {
        initialize_data();
    }
}

// Main function for testing
int main() {
    printf("Content-Type: text/html\n\n");
    printf("<!DOCTYPE html>\n");
    printf("<html><head><title>Pharmacy Management System</title></head><body>\n");
    printf("<h1>🏥 Pharmacy Management System - C Backend</h1>\n");
    printf("<p>Backend is running successfully!</p>\n");
    printf("<p>Data structures initialized with sample data.</p>\n");
    printf("</body></html>\n");
    
    return 0;
}



