# Pharmacy Medicine Management System v2

A complete and visually attractive Pharmacy Medicine Management System with three fully connected components: frontend (HTML), backend (C), and MySQL database.

## 🏥 Overview

This comprehensive Pharmacy Management System provides a complete healthcare solution with modern, elegant design and robust functionality. The system features warm color combinations, soft gradients, aesthetic backgrounds, rounded cards, smooth hover effects, and a clean dashboard-style interface.

## ✨ Features

### 🔐 Authentication System
- Secure login with username/password authentication
- Role-based access control (Administrator and Pharmacist roles)
- Session management and logout functionality

### 💊 Medicine Management
- Add, edit, search, and categorize medicines
- Complete inventory tracking with batch numbers
- Expiry date monitoring and alerts
- Rack location management
- Purchase and selling price tracking

### 📦 Inventory Management
- Real-time stock level tracking
- Automatic stock updates after billing
- Low stock alerts and notifications
- Min/max stock level management
- Stock transaction history

### 💰 Billing and Sales Management
- Generate professional bills and invoices
- Save complete invoice history
- Automatic stock reduction after sales
- Multiple payment methods support (Cash, Card, UPI, Net Banking)
- Tax calculation and itemized billing

### 👥 Patient Records Management
- Store comprehensive patient details
- Medical history tracking
- Allergy information management
- Purchase history for each patient
- Emergency contact information

### ⚠️ Expiry and Stock Alerts
- Real-time monitoring of expiring medicines
- Low stock warnings with reorder suggestions
- Color-coded alert system
- Automated notification generation

### 📊 Analytics Dashboard
- Daily and monthly sales reports
- Top-selling medicines analysis
- Inventory summary with categorization
- Revenue tracking and trends
- Visual statistics with charts

### 🏢 Supplier Management
- Complete supplier database
- Contact information management
- Purchase order tracking
- GST number and address details
- Supplier performance metrics

### 🔔 Notification System
- Internal alerts for low stock
- Expiry warnings notifications
- Billing confirmations
- Daily summary reports
- Priority-based notification management

## 🎨 Design Features

- **Modern & Elegant**: Beautiful gradient backgrounds and soft color schemes
- **No Blue Colors**: Warm color palette with oranges, yellows, greens, and purples
- **Responsive Design**: Fully responsive layout for all device sizes
- **Smooth Animations**: Hover effects and transitions for enhanced UX
- **Rounded Cards**: Modern card-based layout with shadows and depth
- **Dashboard Interface**: Clean, organized dashboard with statistics cards

## 📁 Project Structure

```
Pharmacy Management System v2/
├── pharmacy.sql          # MySQL database schema
├── pharmacy.c            # C backend implementation
├── pharmacy.html         # Frontend interface
└── README.md            # Project documentation
```

## 🚀 Installation & Setup

### Prerequisites
- MySQL Server (version 5.7 or higher)
- GCC Compiler (for C backend)
- Modern web browser (for frontend)
- MySQL C Connector library

### Database Setup

1. **Install MySQL Server**
   ```bash
   # On Ubuntu/Debian
   sudo apt-get install mysql-server
   
   # On Windows
   # Download and install MySQL from https://dev.mysql.com/downloads/mysql/
   ```

2. **Create Database**
   ```sql
   CREATE DATABASE pharmacy_management_v2;
   ```

3. **Import Database Schema**
   ```bash
   mysql -u root -p pharmacy_management_v2 < pharmacy.sql
   ```

### Backend Setup

1. **Install MySQL C Connector**
   ```bash
   # On Ubuntu/Debian
   sudo apt-get install libmysqlclient-dev
   
   # On Windows
   # Download MySQL C Connector from official website
   ```

2. **Compile the Backend**
   ```bash
   gcc pharmacy.c -o pharmacy -lmysqlclient
   ```

3. **Run the Backend**
   ```bash
   ./pharmacy
   ```

### Frontend Setup

1. **Open in Browser**
   - Simply open `pharmacy.html` in any modern web browser
   - No additional setup required

## 🔑 Default Credentials

| Username | Password | Role |
|----------|----------|------|
| admin | admin123 | Administrator |
| pharmacist1 | pharma123 | Pharmacist |

## 📊 Database Schema

### Tables
- `users` - User authentication and roles
- `categories` - Medicine categories
- `suppliers` - Supplier information
- `medicines` - Medicine inventory
- `patients` - Patient records
- `sales` - Sales transactions
- `sale_items` - Individual sale items
- `stock_transactions` - Stock movement tracking
- `purchase_orders` - Purchase order management
- `purchase_order_items` - Purchase order details
- `notifications` - System notifications
- `system_settings` - Configuration settings

### Views
- `medicine_inventory_view` - Complete medicine inventory
- `sales_summary_view` - Sales analytics
- `top_selling_medicines_view` - Best-selling products

### Stored Procedures
- `add_medicine` - Add new medicine with validation
- `process_sale` - Process sales transactions
- `generate_daily_summary` - Generate daily reports

### Triggers
- `check_medicine_alerts` - Automatic alert generation

## 🔧 Technical Specifications

### Backend (C)
- **Language**: C99 standard
- **Database**: MySQL with prepared statements
- **Security**: Input validation, SQL injection prevention
- **Features**: Menu-driven CLI, error handling, logging

### Frontend (HTML/CSS/JavaScript)
- **HTML5**: Semantic markup
- **CSS3**: Modern styling with CSS Grid and Flexbox
- **JavaScript**: ES6+ features, event handling
- **Design**: Responsive, mobile-first approach

### Database (MySQL)
- **Version**: MySQL 5.7+ compatible
- **Engine**: InnoDB for ACID compliance
- **Indexes**: Optimized for performance
- **Constraints**: Foreign keys and data integrity

## 📱 Usage Guide

### 1. Login
- Open `pharmacy.html` in your browser
- Enter default credentials or create new user
- Select appropriate role

### 2. Dashboard Navigation
- View real-time statistics
- Access all modules from dashboard
- Monitor alerts and notifications

### 3. Medicine Management
- Add new medicines with complete details
- Search and filter medicines
- Update stock levels
- Monitor expiry dates

### 4. Billing Process
- Create new sales transactions
- Add multiple items to cart
- Apply taxes and discounts
- Generate professional invoices

### 5. Patient Management
- Register new patients
- Update patient information
- View purchase history
- Track medical records

## 🔒 Security Features

- **Input Validation**: Strict validation for all user inputs
- **SQL Injection Prevention**: Prepared statements and parameterized queries
- **Authentication**: Secure login with role-based access
- **Data Integrity**: Foreign key constraints and validation
- **Error Handling**: Comprehensive error management

## 🌟 Integration Notes

### Frontend-Backend Communication
- Currently uses simulated data for demonstration
- Can be extended with REST API endpoints
- Real-time updates via WebSocket or polling

### Database Connectivity
- Backend uses MySQL C Connector
- Connection pooling for performance
- Automatic reconnection on failure

### Data Flow
1. Frontend captures user input
2. Backend processes and validates data
3. Database stores/retrieves information
4. Frontend displays results with updates

## 🚀 Future Enhancements

### Planned Features
- RESTful API for frontend-backend communication
- Real-time notifications with WebSocket
- Barcode scanning integration
- Prescription management
- Advanced reporting with charts
- Mobile application
- Cloud deployment support

### Performance Optimizations
- Database query optimization
- Caching implementation
- Load balancing support
- Database sharding for large scale

## 🐛 Troubleshooting

### Common Issues

1. **Database Connection Error**
   - Check MySQL server status
   - Verify connection credentials
   - Ensure database exists

2. **Compilation Errors**
   - Install MySQL C Connector
   - Check library paths
   - Verify GCC installation

3. **Frontend Issues**
   - Use modern browser (Chrome, Firefox, Safari)
   - Enable JavaScript
   - Clear browser cache

### Support

For issues and questions:
1. Check the troubleshooting section
2. Review database logs
3. Verify system requirements
4. Contact development team

## 📄 License

This project is provided as-is for educational and demonstration purposes. Please modify and adapt according to your specific requirements.

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create feature branch
3. Test thoroughly
4. Submit pull request

---

**Note**: This is a comprehensive demonstration system. For production use, please implement additional security measures, backup strategies, and performance optimizations.
