-- Pharmacy Management System Database Schema
-- Final version with all requested changes
-- Compatible with C backend implementation

-- Drop existing tables if they exist
DROP TABLE IF EXISTS sale_items;
DROP TABLE IF EXISTS sales;
DROP TABLE IF EXISTS patients;
DROP TABLE IF EXISTS medicines;
DROP TABLE IF EXISTS suppliers;
DROP TABLE IF EXISTS categories;
DROP TABLE IF EXISTS users;

-- Create users table
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,
    name VARCHAR(100) NOT NULL,
    role VARCHAR(20) DEFAULT 'pharmacist',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create categories table
CREATE TABLE categories (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) UNIQUE NOT NULL,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create suppliers table
CREATE TABLE suppliers (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    contact_person VARCHAR(100),
    phone VARCHAR(20),
    email VARCHAR(100),
    address TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create medicines table
CREATE TABLE medicines (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    description TEXT,
    category_id INT,
    supplier_id INT,
    stock_quantity INT DEFAULT 0,
    unit_price DECIMAL(10,2) NOT NULL,
    selling_price DECIMAL(10,2) NOT NULL,
    expiry_date DATE,
    manufacturer VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (category_id) REFERENCES categories(id),
    FOREIGN KEY (supplier_id) REFERENCES suppliers(id),
    INDEX idx_medicine_name (name),
    INDEX idx_category (category_id),
    INDEX idx_supplier (supplier_id),
    INDEX idx_stock (stock_quantity)
);

-- Create patients table
CREATE TABLE patients (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    age INT NOT NULL,
    gender ENUM('पुरुष', 'महिला', 'अन्य') NOT NULL,
    phone VARCHAR(15) UNIQUE NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    address TEXT,
    registration_date DATE DEFAULT (CURRENT_DATE),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_patient_name (name),
    INDEX idx_phone (phone),
    INDEX idx_email (email),
    INDEX idx_registration_date (registration_date)
);

-- Create sales table
CREATE TABLE sales (
    id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT NOT NULL,
    total_amount DECIMAL(10,2) NOT NULL,
    discount_amount DECIMAL(10,2) DEFAULT 0.00,
    gst_amount DECIMAL(10,2) DEFAULT 0.00,
    final_amount DECIMAL(10,2) NOT NULL,
    payment_method ENUM('कैश', 'कार्ड', 'UPI', 'चेक') NOT NULL,
    payment_status ENUM('लंबित', 'भुगतान हो गया', 'रद्द') DEFAULT 'भुगतान हो गया',
    sale_date DATE DEFAULT (CURRENT_DATE),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (patient_id) REFERENCES patients(id),
    INDEX idx_patient (patient_id),
    INDEX idx_sale_date (sale_date),
    INDEX idx_payment_status (payment_status)
);

-- Create sale_items table
CREATE TABLE sale_items (
    id INT AUTO_INCREMENT PRIMARY KEY,
    sale_id INT NOT NULL,
    medicine_id INT NOT NULL,
    quantity INT NOT NULL,
    unit_price DECIMAL(10,2) NOT NULL,
    total_price DECIMAL(10,2) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (sale_id) REFERENCES sales(id) ON DELETE CASCADE,
    FOREIGN KEY (medicine_id) REFERENCES medicines(id),
    INDEX idx_sale (sale_id),
    INDEX idx_medicine (medicine_id)
);

-- Create payment_queue table
CREATE TABLE payment_queue (
    id INT AUTO_INCREMENT PRIMARY KEY,
    bill_id INT NOT NULL,
    patient_id INT NOT NULL,
    total_amount DECIMAL(10,2) NOT NULL,
    payment_method ENUM('कैश', 'कार्ड', 'UPI', 'चेक') NOT NULL,
    priority INT DEFAULT 3 COMMENT '1=High, 2=Medium, 3=Low',
    queue_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('लंबित', 'प्रोसेसिंग', 'भुगतान हो गया', 'रद्द') DEFAULT 'लंबित',
    processed_at TIMESTAMP NULL,
    FOREIGN KEY (patient_id) REFERENCES patients(id),
    INDEX idx_priority (priority),
    INDEX idx_status (status),
    INDEX idx_queue_date (queue_date)
);

-- Create stock_transactions table
CREATE TABLE stock_transactions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    medicine_id INT NOT NULL,
    transaction_type ENUM('खरीद', 'बिक्री', 'रिटर्न', 'वापसी', 'एडजस्टमेंट') NOT NULL,
    quantity INT NOT NULL,
    unit_price DECIMAL(10,2),
    reference_id INT COMMENT 'Reference to sale or purchase ID',
    notes TEXT,
    transaction_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (medicine_id) REFERENCES medicines(id),
    INDEX idx_medicine (medicine_id),
    INDEX idx_transaction_type (transaction_type),
    INDEX idx_transaction_date (transaction_date)
);

-- Insert sample categories
INSERT INTO categories (name, description) VALUES
('दर्द निवारक', 'Pain relief medications'),
('एंटीबायोटिक', 'Antibiotic medications'),
('विटामिन', 'Vitamin supplements'),
('एलर्जी', 'Allergy medications'),
('डायबिटीज', 'Diabetes medications'),
('दिल की बीमारी', 'Heart disease medications'),
('त्वचा', 'Skin care medications'),
('आँख', 'Eye care medications');

-- Insert sample suppliers with Indian names
INSERT INTO suppliers (name, contact_person, phone, email, address) VALUES
('राजेंद्र मेडिकल', 'राजेश गुप्ता', '9876543210', 'rajendra@medical.com', '123 मुखर्जी नगर, दिल्ली'),
('सुरभि फार्मा', 'सुरभि सिंह', '9876543211', 'surabhi@pharma.com', '456 बांद्रा, मुंबई'),
('सन फार्मा', 'अमित सूर्यवंशी', '9876543212', 'sun@pharma.com', '789 साल्ट लेक, कोलकाता'),
('सिप्ला', 'प्रिया शर्मा', '9876543213', 'cipla@pharma.com', '321 इंदिरानगर, बैंगलोर'),
('रेड्डीज लैब', 'विक्रम रेड्डी', '9876543214', 'reddy@labs.com', '654 जयपुर, राजस्थान');

-- Insert sample medicines with Indian names
INSERT INTO medicines (name, description, category_id, supplier_id, stock_quantity, unit_price, selling_price, expiry_date, manufacturer) VALUES
('Paracetamol 500mg', 'Pain relief and fever reducer', 1, 1, 100, 5.50, 8.99, '2025-12-31', 'राजेंद्र मेडिकल'),
('Azithromycin 250mg', 'Antibiotic for bacterial infections', 2, 2, 50, 15.00, 25.99, '2025-06-15', 'सुरभि फार्मा'),
('Dolo 650mg', 'Pain relief medication', 1, 3, 75, 7.00, 12.50, '2025-09-20', 'सन फार्मा'),
('Amoxicillin 500mg', 'Broad spectrum antibiotic', 2, 4, 60, 12.00, 20.99, '2025-12-10', 'सिप्ला'),
('Crocin 500mg', 'Fever and pain relief', 1, 5, 80, 6.50, 10.99, '2025-08-25', 'रेड्डीज लैब'),
('Vitamin D3', 'Vitamin D supplement', 3, 1, 120, 8.00, 15.99, '2026-03-15', 'राजेंद्र मेडिकल'),
('Allegra 180mg', 'Allergy relief medication', 4, 2, 40, 10.00, 18.99, '2025-11-30', 'सुरभि फार्मा'),
('Metformin 500mg', 'Diabetes medication', 5, 3, 90, 4.50, 9.99, '2026-01-20', 'सन फार्मा'),
('Aspirin 75mg', 'Heart disease prevention', 6, 4, 150, 3.00, 6.99, '2025-10-15', 'सिप्ला'),
('Betnovate', 'Skin cream for eczema', 7, 5, 30, 25.00, 45.99, '2025-07-10', 'रेड्डीज लैब'),
('Lumify', 'Eye drops for redness', 8, 1, 25, 35.00, 65.99, '2025-09-05', 'राजेंद्र मेडिकल'),
('Ibuprofen 400mg', 'Anti-inflammatory pain relief', 1, 2, 85, 4.00, 7.99, '2025-11-20', 'सुरभि फार्मा');

-- Insert sample patients with Indian names
INSERT INTO patients (name, age, gender, phone, email, address, registration_date) VALUES
('राजेश कुमार', 35, 'पुरुष', '9876543210', 'rajesh.kumar@email.com', '123 मुखर्जी नगर, दिल्ली', '2024-01-01'),
('प्रिया शर्मा', 28, 'महिला', '9876543211', 'priya.sharma@email.com', '456 बांद्रा, मुंबई', '2024-01-02'),
('अमित सिंह', 42, 'पुरुष', '9876543212', 'amit.singh@email.com', '789 साल्ट लेक, कोलकाता', '2024-01-03'),
('नेहा गुप्ता', 31, 'महिला', '9876543213', 'neha.gupta@email.com', '321 इंदिरानगर, बैंगलोर', '2024-01-04'),
('विक्रम राठौर', 38, 'पुरुष', '9876543214', 'vikram.rathore@email.com', '654 जयपुर, राजस्थान', '2024-01-05'),
('अंजलि देवी', 26, 'महिला', '9876543215', 'anjali.devi@email.com', '987 चेन्नई, तमिलनाडु', '2024-01-06'),
('राहुल वर्मा', 45, 'पुरुष', '9876543216', 'rahul.verma@email.com', '654 लखनऊ, उत्तर प्रदेश', '2024-01-07'),
('कविता मिश्रा', 33, 'महिला', '9876543217', 'kavita.mishra@email.com', '321 पुणे, महाराष्ट्र', '2024-01-08'),
('सुरेश यादव', 40, 'पुरुष', '9876543218', 'suresh.yadav@email.com', '147 हैदराबाद, तेलंगाना', '2024-01-09'),
('पूजा नायर', 29, 'महिला', '9876543219', 'poja.nair@email.com', '258 कोचीन, केरल', '2024-01-10');

-- Insert sample users
INSERT INTO users (username, password, name, role) VALUES
('admin', 'admin123', 'Administrator', 'admin'),
('pharmacist1', 'pharm123', 'राजेश कुमार', 'pharmacist'),
('pharmacist2', 'pharm456', 'प्रिया शर्मा', 'pharmacist');

-- Insert sample sales data
INSERT INTO sales (patient_id, total_amount, discount_amount, gst_amount, final_amount, payment_method, payment_status, sale_date) VALUES
(1, 125.50, 5.00, 11.25, 131.75, 'कैश', 'भुगतान हो गया', '2024-01-15'),
(2, 89.99, 0.00, 8.10, 98.09, 'कार्ड', 'भुगतान हो गया', '2024-01-16'),
(3, 156.75, 10.00, 13.15, 159.90, 'UPI', 'भुगतान हो गया', '2024-01-17'),
(4, 234.99, 15.00, 21.90, 241.89, 'कैश', 'भुगतान हो गया', '2024-01-18'),
(5, 78.50, 2.50, 6.90, 82.90, 'कार्ड', 'भुगतान हो गया', '2024-01-19');

-- Insert sample sale items
INSERT INTO sale_items (sale_id, medicine_id, quantity, unit_price, total_price) VALUES
(1, 1, 5, 8.99, 44.95),
(1, 3, 6, 12.50, 75.00),
(1, 6, 3, 15.99, 47.97),
(2, 2, 2, 25.99, 51.98),
(2, 4, 3, 20.99, 62.97),
(2, 7, 1, 18.99, 18.99),
(3, 1, 8, 8.99, 71.92),
(3, 5, 7, 10.99, 76.93),
(3, 8, 4, 9.99, 39.96),
(4, 2, 5, 25.99, 129.95),
(4, 6, 6, 15.99, 95.94),
(4, 9, 8, 6.99, 55.92),
(5, 3, 4, 12.50, 50.00),
(5, 5, 2, 10.99, 21.98),
(5, 10, 1, 45.99, 45.99);

-- Insert sample payment queue data
INSERT INTO payment_queue (bill_id, patient_id, total_amount, payment_method, priority, status) VALUES
(6, 1, 145.75, 'UPI', 2, 'लंबित'),
(7, 2, 289.99, 'कार्ड', 1, 'लंबित'),
(8, 3, 67.50, 'कैश', 3, 'प्रोसेसिंग'),
(9, 4, 123.25, 'UPI', 2, 'भुगतान हो गया'),
(10, 5, 198.00, 'कार्ड', 1, 'लंबित');

-- Insert sample stock transactions
INSERT INTO stock_transactions (medicine_id, transaction_type, quantity, unit_price, reference_id, notes) VALUES
(1, 'खरीद', 200, 5.50, NULL, 'Initial stock purchase'),
(2, 'खरीद', 100, 15.00, NULL, 'Initial stock purchase'),
(3, 'खरीद', 150, 7.00, NULL, 'Initial stock purchase'),
(4, 'खरीद', 120, 12.00, NULL, 'Initial stock purchase'),
(5, 'खरीद', 160, 6.50, NULL, 'Initial stock purchase'),
(1, 'बिक्री', 5, 8.99, 1, 'Sale to patient'),
(2, 'बिक्री', 2, 25.99, 2, 'Sale to patient'),
(3, 'बिक्री', 6, 12.50, 1, 'Sale to patient'),
(4, 'बिक्री', 3, 20.99, 2, 'Sale to patient'),
(5, 'बिक्री', 7, 10.99, 3, 'Sale to patient');

-- Create views for reporting
CREATE VIEW patient_summary AS
SELECT 
    p.id,
    p.name,
    p.age,
    p.gender,
    p.phone,
    p.email,
    p.registration_date,
    COUNT(s.id) as total_sales,
    COALESCE(SUM(s.final_amount), 0) as total_spent
FROM patients p
LEFT JOIN sales s ON p.id = s.patient_id
GROUP BY p.id, p.name, p.age, p.gender, p.phone, p.email, p.registration_date;

CREATE VIEW medicine_summary AS
SELECT 
    m.id,
    m.name,
    m.category_id,
    c.name as category_name,
    m.stock_quantity,
    m.unit_price,
    m.selling_price,
    m.expiry_date,
    m.supplier_id,
    s.name as supplier_name,
    COALESCE(SUM(st.quantity), 0) as total_sold,
    COALESCE(SUM(st.quantity * st.unit_price), 0) as total_revenue
FROM medicines m
LEFT JOIN categories c ON m.category_id = c.id
LEFT JOIN suppliers s ON m.supplier_id = s.id
LEFT JOIN stock_transactions st ON m.id = st.medicine_id AND st.transaction_type = 'बिक्री'
GROUP BY m.id, m.name, m.category_id, c.name, m.stock_quantity, m.unit_price, m.selling_price, m.expiry_date, m.supplier_id, s.name;

CREATE VIEW monthly_sales_report AS
SELECT 
    DATE_FORMAT(sale_date, '%Y-%m') as month,
    COUNT(*) as total_sales,
    SUM(final_amount) as total_revenue,
    AVG(final_amount) as average_sale,
    COUNT(DISTINCT patient_id) as unique_patients
FROM sales
WHERE payment_status = 'भुगतान हो गया'
GROUP BY DATE_FORMAT(sale_date, '%Y-%m')
ORDER BY month DESC;

-- Create stored procedures for common operations
DELIMITER //

CREATE PROCEDURE add_medicine(
    IN p_name VARCHAR(200),
    IN p_description TEXT,
    IN p_category_id INT,
    IN p_supplier_id INT,
    IN p_stock_quantity INT,
    IN p_unit_price DECIMAL(10,2),
    IN p_selling_price DECIMAL(10,2),
    IN p_expiry_date DATE,
    IN p_manufacturer VARCHAR(100)
)
BEGIN
    INSERT INTO medicines (name, description, category_id, supplier_id, stock_quantity, unit_price, selling_price, expiry_date, manufacturer)
    VALUES (p_name, p_description, p_category_id, p_supplier_id, p_stock_quantity, p_unit_price, p_selling_price, p_expiry_date, p_manufacturer);
    
    -- Record stock transaction
    INSERT INTO stock_transactions (medicine_id, transaction_type, quantity, unit_price, notes)
    VALUES (LAST_INSERT_ID(), 'खरीद', p_stock_quantity, p_unit_price, 'Initial stock entry');
END//

CREATE PROCEDURE add_patient(
    IN p_name VARCHAR(100),
    IN p_age INT,
    IN p_gender ENUM('पुरुष', 'महिला', 'अन्य'),
    IN p_phone VARCHAR(15),
    IN p_email VARCHAR(100),
    IN p_address TEXT
)
BEGIN
    INSERT INTO patients (name, age, gender, phone, email, address)
    VALUES (p_name, p_age, p_gender, p_phone, p_email, p_address);
END//

CREATE PROCEDURE create_sale(
    IN p_patient_id INT,
    IN p_total_amount DECIMAL(10,2),
    IN p_discount_amount DECIMAL(10,2),
    IN p_gst_amount DECIMAL(10,2),
    IN p_final_amount DECIMAL(10,2),
    IN p_payment_method ENUM('कैश', 'कार्ड', 'UPI', 'चेक')
)
BEGIN
    DECLARE sale_id INT;
    
    -- Create sale record
    INSERT INTO sales (patient_id, total_amount, discount_amount, gst_amount, final_amount, payment_method)
    VALUES (p_patient_id, p_total_amount, p_discount_amount, p_gst_amount, p_final_amount, p_payment_method);
    
    SET sale_id = LAST_INSERT_ID();
    
    -- Add to payment queue if payment is pending
    IF p_payment_method IN ('कार्ड', 'UPI') THEN
        INSERT INTO payment_queue (bill_id, patient_id, total_amount, payment_method, priority, status)
        VALUES (sale_id, p_patient_id, p_final_amount, p_payment_method, 
                CASE WHEN p_final_amount > 1000 THEN 1 WHEN p_final_amount > 500 THEN 2 ELSE 3 END, 'लंबित');
    END IF;
    
    SELECT sale_id;
END//

CREATE PROCEDURE add_sale_item(
    IN p_sale_id INT,
    IN p_medicine_id INT,
    IN p_quantity INT,
    IN p_unit_price DECIMAL(10,2),
    IN p_total_price DECIMAL(10,2)
)
BEGIN
    -- Add sale item
    INSERT INTO sale_items (sale_id, medicine_id, quantity, unit_price, total_price)
    VALUES (p_sale_id, p_medicine_id, p_quantity, p_unit_price, p_total_price);
    
    -- Update medicine stock
    UPDATE medicines 
    SET stock_quantity = stock_quantity - p_quantity 
    WHERE id = p_medicine_id;
    
    -- Record stock transaction
    INSERT INTO stock_transactions (medicine_id, transaction_type, quantity, unit_price, reference_id, notes)
    VALUES (p_medicine_id, 'बिक्री', p_quantity, p_unit_price, p_sale_id, 'Sale transaction');
END//

CREATE PROCEDURE process_payment(IN p_queue_id INT)
BEGIN
    DECLARE v_bill_id INT;
    DECLARE v_patient_id INT;
    
    -- Get payment details
    SELECT bill_id, patient_id INTO v_bill_id, v_patient_id
    FROM payment_queue WHERE id = p_queue_id;
    
    -- Update payment queue status
    UPDATE payment_queue 
    SET status = 'भुगतान हो गया', processed_at = CURRENT_TIMESTAMP 
    WHERE id = p_queue_id;
    
    -- Update sale payment status
    UPDATE sales 
    SET payment_status = 'भुगतान हो गया' 
    WHERE id = v_bill_id;
END//

DELIMITER ;

-- Create triggers for data integrity
DELIMITER //

CREATE TRIGGER before_medicine_update 
BEFORE UPDATE ON medicines
FOR EACH ROW
BEGIN
    -- Log stock changes
    IF OLD.stock_quantity != NEW.stock_quantity THEN
        IF NEW.stock_quantity < OLD.stock_quantity THEN
            INSERT INTO stock_transactions (medicine_id, transaction_type, quantity, unit_price, notes)
            VALUES (NEW.id, 'बिक्री', OLD.stock_quantity - NEW.stock_quantity, NEW.selling_price, 'Stock reduction');
        ELSE
            INSERT INTO stock_transactions (medicine_id, transaction_type, quantity, unit_price, notes)
            VALUES (NEW.id, 'खरीद', NEW.stock_quantity - OLD.stock_quantity, NEW.unit_price, 'Stock increase');
        END IF;
    END IF;
END//

CREATE TRIGGER check_low_stock
AFTER UPDATE ON medicines
FOR EACH ROW
BEGIN
    -- Alert when stock is low (less than 20)
    IF NEW.stock_quantity < 20 AND NEW.stock_quantity >= 0 THEN
        -- In a real system, this would send a notification
        SELECT CONCAT('LOW STOCK ALERT: ', NEW.name, ' has only ', NEW.stock_quantity, ' units remaining') as alert;
    END IF;
END//

DELIMITER ;

-- Create indexes for performance optimization
CREATE INDEX idx_composite_patient_sales ON patients(id, registration_date);
CREATE INDEX idx_composite_medicine_sales ON medicines(id, category_id, stock_quantity);
CREATE INDEX idx_composite_sales_date ON sales(sale_date, payment_status);
CREATE INDEX idx_composite_queue_priority ON payment_queue(priority, status, queue_date);

-- Final summary
SELECT 'Database setup completed successfully!' as message;
SELECT COUNT(*) as total_categories FROM categories;
SELECT COUNT(*) as total_suppliers FROM suppliers;
SELECT COUNT(*) as total_medicines FROM medicines;
SELECT COUNT(*) as total_patients FROM patients;
SELECT COUNT(*) as total_sales FROM sales;
SELECT COUNT(*) as total_payment_queue FROM payment_queue;
