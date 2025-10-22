-- Pharmacy Management System Database Schema
-- SQLite Database Creation and Initialization Script

-- Enable foreign key constraints
PRAGMA foreign_keys = ON;

-- Create Users table
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username VARCHAR(80) UNIQUE NOT NULL,
    email VARCHAR(120) UNIQUE NOT NULL,
    password_hash VARCHAR(128) NOT NULL,
    role VARCHAR(20) NOT NULL CHECK (role IN ('admin', 'pharmacist')),
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    phone VARCHAR(20),
    is_active BOOLEAN DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Create Medicines table
CREATE TABLE IF NOT EXISTS medicines (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(100) NOT NULL,
    generic_name VARCHAR(100),
    manufacturer VARCHAR(100),
    batch_number VARCHAR(50),
    expiry_date DATE NOT NULL,
    quantity INTEGER NOT NULL,
    unit_price DECIMAL(10,2) NOT NULL,
    category VARCHAR(50),
    description TEXT,
    minimum_stock_level INTEGER DEFAULT 10,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Create Sales table
CREATE TABLE IF NOT EXISTS sales (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    total_amount DECIMAL(10,2) NOT NULL,
    sale_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

-- Create Sale Items table
CREATE TABLE IF NOT EXISTS sale_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sale_id INTEGER NOT NULL,
    medicine_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    unit_price DECIMAL(10,2) NOT NULL,
    total_price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (sale_id) REFERENCES sales(id),
    FOREIGN KEY (medicine_id) REFERENCES medicines(id)
);

-- Create indexes for better performance
CREATE INDEX IF NOT EXISTS idx_users_username ON users(username);
CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);
CREATE INDEX IF NOT EXISTS idx_users_role ON users(role);
CREATE INDEX IF NOT EXISTS idx_medicines_name ON medicines(name);
CREATE INDEX IF NOT EXISTS idx_medicines_category ON medicines(category);
CREATE INDEX IF NOT EXISTS idx_medicines_expiry ON medicines(expiry_date);
CREATE INDEX IF NOT EXISTS idx_sales_user_id ON sales(user_id);
CREATE INDEX IF NOT EXISTS idx_sales_date ON sales(sale_date);
CREATE INDEX IF NOT EXISTS idx_sale_items_sale_id ON sale_items(sale_id);
CREATE INDEX IF NOT EXISTS idx_sale_items_medicine_id ON sale_items(medicine_id);

-- Create triggers for updated_at timestamps
CREATE TRIGGER IF NOT EXISTS update_users_timestamp 
    AFTER UPDATE ON users
    BEGIN
        UPDATE users SET updated_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
    END;

CREATE TRIGGER IF NOT EXISTS update_medicines_timestamp 
    AFTER UPDATE ON medicines
    BEGIN
        UPDATE medicines SET updated_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
    END;

-- Insert sample data
-- Admin user (password: admin123)
INSERT OR IGNORE INTO users (username, email, password_hash, role, first_name, last_name, phone) 
VALUES ('admin', 'admin@pharmacy.com', 'scrypt:32768:8:1$2b$12$8K1p/a0dL1x5n8K1p/a0dL1x5n8K1p/a0dL1x5n8K1p/a0dL1x5n8K1p', 'admin', 'Admin', 'User', '123-456-7890');

-- Pharmacist user (password: pharmacist123)
INSERT OR IGNORE INTO users (username, email, password_hash, role, first_name, last_name, phone) 
VALUES ('pharmacist', 'pharmacist@pharmacy.com', 'scrypt:32768:8:1$2b$12$8K1p/a0dL1x5n8K1p/a0dL1x5n8K1p/a0dL1x5n8K1p/a0dL1x5n8K1p', 'pharmacist', 'John', 'Pharmacist', '123-456-7891');

-- Sample medicines
INSERT OR IGNORE INTO medicines (name, generic_name, manufacturer, batch_number, expiry_date, quantity, unit_price, category, description, minimum_stock_level) VALUES
('Paracetamol', 'Acetaminophen', 'PharmaCorp', 'PC001', '2025-12-31', 100, 5.50, 'Painkiller', 'Pain relief medication', 20),
('Amoxicillin', 'Amoxicillin', 'MediCorp', 'AM001', '2024-06-30', 5, 15.75, 'Antibiotic', 'Antibiotic for bacterial infections', 10),
('Vitamin D', 'Cholecalciferol', 'VitCorp', 'VD001', '2025-03-15', 50, 12.00, 'Vitamin', 'Vitamin D supplement', 15),
('Omeprazole', 'Omeprazole', 'GastroCorp', 'OM001', '2024-08-20', 25, 8.50, 'Antacid', 'Proton pump inhibitor', 10),
('Ibuprofen', 'Ibuprofen', 'PainCorp', 'IB001', '2025-01-10', 75, 6.25, 'Painkiller', 'Anti-inflammatory pain reliever', 20),
('Aspirin', 'Acetylsalicylic acid', 'CardioCorp', 'AS001', '2025-05-20', 60, 4.75, 'Painkiller', 'Cardiovascular protection', 15),
('Metformin', 'Metformin', 'DiabCorp', 'MF001', '2024-11-15', 30, 18.50, 'Other', 'Diabetes medication', 10),
('Lisinopril', 'Lisinopril', 'HeartCorp', 'LS001', '2025-07-30', 40, 22.00, 'Other', 'ACE inhibitor for blood pressure', 12),
('Simvastatin', 'Simvastatin', 'CholCorp', 'SV001', '2025-09-10', 35, 25.75, 'Other', 'Cholesterol lowering medication', 10),
('Cetirizine', 'Cetirizine', 'AllerCorp', 'CT001', '2024-12-05', 80, 7.25, 'Other', 'Antihistamine for allergies', 20);
