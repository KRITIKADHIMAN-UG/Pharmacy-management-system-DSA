# Pharmacy Management System - Complete Project Documentation

## 🏥 Project Overview
This is a complete Pharmacy Management System that integrates:
- **C Backend** - Core data structures and business logic
- **HTML/CSS/JS Frontend** - Modern web interface
- **SQLite Database** - Persistent data storage
- **Python HTTP Server** - API bridge between frontend and database

## 📁 Project Structure
```
Pharmacy/
├── backend.c              # C backend with data structures
├── frontend.html           # Complete web interface
├── database.sql            # SQLite database schema
├── project_official.sh     # Linux/Mac runner script
├── project_official.bat    # Windows runner script
└── README.md               # This documentation
```

## 🚀 How to Run the Project

### For Windows Users:
1. **Double-click** `project_official.bat`
2. The script will automatically:
   - Check requirements (GCC, SQLite3, Python)
   - Compile the C backend
   - Initialize the database
   - Start the HTTP server
3. **Open your browser** and go to `http://localhost:8000`

### For Linux/Mac Users:
1. **Open terminal** in the project directory
2. **Run**: `chmod +x project_official.sh && ./project_official.sh`
3. The script will automatically set everything up
4. **Open your browser** and go to `http://localhost:8000`

## 🔧 Requirements
- **GCC Compiler** (MinGW for Windows, gcc for Linux/Mac)
- **SQLite3** database
- **Python 3** (for HTTP server)
- **Modern Web Browser** (Chrome, Firefox, Safari, Edge)

## 🔑 Test Credentials
- **Admin**: username=`admin`, password=`admin123`
- **Doctor**: username=`doctor`, password=`doctor123`
- **Patient**: username=`patient`, password=`patient123`

## ✨ Features

### 🎯 **Core Functionality**
- **User Authentication** - Role-based login system
- **Patient Management** - Add, view, update patient records
- **Doctor Management** - Manage doctor profiles and specializations
- **Bed Management** - Track bed occupancy and assignments
- **Medicine Management** - Inventory tracking with expiry alerts
- **Dashboard Analytics** - Real-time statistics and summaries

### 🏗️ **Architecture**
- **C Backend** - High-performance data structures and algorithms
- **SQLite Database** - Reliable, file-based database
- **HTML/CSS/JS Frontend** - Responsive, modern web interface
- **Python HTTP Server** - RESTful API bridge
- **Cross-Platform** - Works on Windows, Linux, and Mac

### 📊 **Database Schema**
- **Users Table** - Authentication and role management
- **Patients Table** - Complete patient information
- **Doctors Table** - Doctor profiles and specializations
- **Beds Table** - Bed tracking with occupancy status
- **Medicines Table** - Medicine inventory with expiry tracking
- **Appointments Table** - Future appointment scheduling
- **Prescriptions Table** - Future prescription management

## 🎮 **How to Use**

### 1. **Login**
- Use the test credentials to log in
- Different roles have different access levels

### 2. **Dashboard**
- View real-time statistics
- Monitor system health and alerts

### 3. **Patient Management**
- Add new patients with medical history
- View all patient records
- Update patient information

### 4. **Doctor Management**
- Add doctors with specializations
- Manage doctor schedules and availability
- Track license numbers

### 5. **Bed Management**
- Add new beds (General, ICU, Emergency)
- Assign beds to patients
- Release beds when patients leave

### 6. **Medicine Management**
- Add medicines with expiry dates
- Track stock levels and low-stock alerts
- Monitor expired medicines

## 🔄 **Running Multiple Times**
The system is designed to run multiple times without issues:
- **Database persists** between runs
- **Sample data** is only created once
- **Compiled C backend** is reused
- **HTTP server** starts fresh each time

## 🛠️ **Troubleshooting**

### Common Issues:
1. **GCC not found** - Install MinGW (Windows) or gcc (Linux/Mac)
2. **SQLite3 not found** - Install SQLite3 from official website
3. **Python not found** - Install Python 3 from python.org
4. **Port 8000 in use** - Close other applications using port 8000

### Solutions:
- **Windows**: Install MinGW-w64, SQLite3, and Python 3
- **Linux**: `sudo apt-get install gcc sqlite3 python3`
- **Mac**: `brew install gcc sqlite python3`

## 📈 **Performance Features**
- **C Data Structures** - Fast memory operations
- **SQLite Indexing** - Optimized database queries
- **Responsive Design** - Works on all screen sizes
- **Real-time Updates** - Live data synchronization

## 🔒 **Security Features**
- **Role-based Access** - Different permissions for different users
- **Input Validation** - Server-side data validation
- **SQL Injection Protection** - Parameterized queries
- **CORS Headers** - Cross-origin request security

## 🌟 **Future Enhancements**
- **Appointment Scheduling** - Book and manage appointments
- **Prescription Management** - Digital prescription system
- **File Uploads** - Medical document storage
- **Real-time Notifications** - WebSocket integration
- **Advanced Reporting** - PDF report generation
- **Mobile App** - React Native mobile interface

## 📞 **Support**
If you encounter any issues:
1. Check that all requirements are installed
2. Ensure no other application is using port 8000
3. Try running the script as administrator (Windows)
4. Check the console output for error messages

## 🎉 **Success Indicators**
When everything is working correctly, you should see:
- ✅ All requirements satisfied
- ✅ C backend compiled successfully
- ✅ Database initialized successfully
- ✅ HTTP server created
- 🌐 Server running at http://localhost:8000
run everything here file:///C:/Users/hlpuy/Desktop/Pharmacy/frontend.html


**Enjoy using your Pharmacy Management System!** 🏥💊
