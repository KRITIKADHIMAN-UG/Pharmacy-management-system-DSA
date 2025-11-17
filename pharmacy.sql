-- Pharmacy Medicine Management System Database Schema
-- Complete database design for modern pharmacy management

CREATE DATABASE IF NOT EXISTS pharmacy_management_v2;
USE pharmacy_management_v2;

-- Users table for authentication
CREATE TABLE users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    full_name VARCHAR(100) NOT NULL,
    email VARCHAR(100),
    phone VARCHAR(10),
    role ENUM('admin', 'pharmacist') DEFAULT 'pharmacist',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP NULL,
    is_active BOOLEAN DEFAULT TRUE
);

-- Categories for medicines
CREATE TABLE categories (
    category_id INT PRIMARY KEY AUTO_INCREMENT,
    category_name VARCHAR(50) UNIQUE NOT NULL,
    description TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Suppliers table
CREATE TABLE suppliers (
    supplier_id INT PRIMARY KEY AUTO_INCREMENT,
    supplier_name VARCHAR(100) NOT NULL,
    contact_person VARCHAR(100),
    phone VARCHAR(10) NOT NULL,
    email VARCHAR(100),
    address TEXT,
    gst_number VARCHAR(15),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_active BOOLEAN DEFAULT TRUE
);

-- Medicines table
CREATE TABLE medicines (
    medicine_id INT PRIMARY KEY AUTO_INCREMENT,
    medicine_name VARCHAR(100) NOT NULL,
    generic_name VARCHAR(100),
    category_id INT,
    supplier_id INT,
    description TEXT,
    strength VARCHAR(50),
    formulation VARCHAR(50),
    batch_number VARCHAR(50) NOT NULL,
    manufacture_date DATE NOT NULL,
    expiry_date DATE NOT NULL,
    purchase_price DECIMAL(10,2) NOT NULL,
    selling_price DECIMAL(10,2) NOT NULL,
    stock_quantity INT DEFAULT 0,
    min_stock_level INT DEFAULT 10,
    reorder_level INT DEFAULT 20,
    rack_location VARCHAR(50),
    storage_conditions TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    is_active BOOLEAN DEFAULT TRUE,
    FOREIGN KEY (category_id) REFERENCES categories(category_id),
    FOREIGN KEY (supplier_id) REFERENCES suppliers(supplier_id)
);

-- Patients table
CREATE TABLE patients (
    patient_id INT PRIMARY KEY AUTO_INCREMENT,
    patient_name VARCHAR(100) NOT NULL,
    age INT,
    gender ENUM('Male', 'Female', 'Other'),
    phone VARCHAR(10) NOT NULL,
    email VARCHAR(100),
    address TEXT,
    medical_history TEXT,
    allergies TEXT,
    emergency_contact VARCHAR(10),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Sales/Billing table
CREATE TABLE sales (
    sale_id INT PRIMARY KEY AUTO_INCREMENT,
    invoice_number VARCHAR(50) UNIQUE NOT NULL,
    patient_id INT,
    user_id INT,
    sale_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    subtotal DECIMAL(10,2) NOT NULL,
    discount_amount DECIMAL(10,2) DEFAULT 0,
    tax_amount DECIMAL(10,2) DEFAULT 0,
    total_amount DECIMAL(10,2) NOT NULL,
    payment_method ENUM('Cash', 'Card', 'UPI', 'Net Banking') DEFAULT 'Cash',
    payment_status ENUM('Paid', 'Pending', 'Refunded') DEFAULT 'Paid',
    notes TEXT,
    FOREIGN KEY (patient_id) REFERENCES patients(patient_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- Sale items table (individual medicines in each sale)
CREATE TABLE sale_items (
    sale_item_id INT PRIMARY KEY AUTO_INCREMENT,
    sale_id INT NOT NULL,
    medicine_id INT NOT NULL,
    quantity INT NOT NULL,
    unit_price DECIMAL(10,2) NOT NULL,
    total_price DECIMAL(10,2) NOT NULL,
    batch_number VARCHAR(50),
    FOREIGN KEY (sale_id) REFERENCES sales(sale_id) ON DELETE CASCADE,
    FOREIGN KEY (medicine_id) REFERENCES medicines(medicine_id)
);

-- Stock transactions table
CREATE TABLE stock_transactions (
    transaction_id INT PRIMARY KEY AUTO_INCREMENT,
    medicine_id INT NOT NULL,
    transaction_type ENUM('Purchase', 'Sale', 'Return', 'Adjustment', 'Expired') NOT NULL,
    quantity INT NOT NULL,
    reference_id INT,
    reference_type ENUM('Purchase', 'Sale', 'Adjustment'),
    transaction_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    user_id INT,
    notes TEXT,
    FOREIGN KEY (medicine_id) REFERENCES medicines(medicine_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- Purchase orders table
CREATE TABLE purchase_orders (
    order_id INT PRIMARY KEY AUTO_INCREMENT,
    order_number VARCHAR(50) UNIQUE NOT NULL,
    supplier_id INT NOT NULL,
    user_id INT,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expected_delivery_date DATE,
    status ENUM('Pending', 'Confirmed', 'Partially Delivered', 'Delivered', 'Cancelled') DEFAULT 'Pending',
    total_amount DECIMAL(10,2) DEFAULT 0,
    notes TEXT,
    FOREIGN KEY (supplier_id) REFERENCES suppliers(supplier_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- Purchase order items table
CREATE TABLE purchase_order_items (
    order_item_id INT PRIMARY KEY AUTO_INCREMENT,
    order_id INT NOT NULL,
    medicine_id INT NOT NULL,
    quantity INT NOT NULL,
    unit_price DECIMAL(10,2) NOT NULL,
    total_price DECIMAL(10,2) NOT NULL,
    delivered_quantity INT DEFAULT 0,
    FOREIGN KEY (order_id) REFERENCES purchase_orders(order_id) ON DELETE CASCADE,
    FOREIGN KEY (medicine_id) REFERENCES medicines(medicine_id)
);

-- Notifications table
CREATE TABLE notifications (
    notification_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT,
    title VARCHAR(200) NOT NULL,
    message TEXT NOT NULL,
    notification_type ENUM('Low Stock', 'Expiry Alert', 'Billing', 'System', 'Daily Summary') NOT NULL,
    priority ENUM('Low', 'Medium', 'High', 'Critical') DEFAULT 'Medium',
    is_read BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expires_at TIMESTAMP,
    action_url VARCHAR(200),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);

-- System settings table
CREATE TABLE system_settings (
    setting_id INT PRIMARY KEY AUTO_INCREMENT,
    setting_key VARCHAR(100) UNIQUE NOT NULL,
    setting_value TEXT,
    description TEXT,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Indexes for performance
CREATE INDEX idx_medicines_name ON medicines(medicine_name);
CREATE INDEX idx_medicines_category ON medicines(category_id);
CREATE INDEX idx_medicines_supplier ON medicines(supplier_id);
CREATE INDEX idx_medicines_expiry ON medicines(expiry_date);
CREATE INDEX idx_medicines_stock ON medicines(stock_quantity);
CREATE INDEX idx_patients_phone ON patients(phone);
CREATE INDEX idx_sales_date ON sales(sale_date);
CREATE INDEX idx_sales_patient ON sales(patient_id);
CREATE INDEX idx_stock_transactions_medicine ON stock_transactions(medicine_id);
CREATE INDEX idx_stock_transactions_date ON stock_transactions(transaction_date);
CREATE INDEX idx_notifications_user ON notifications(user_id);
CREATE INDEX idx_notifications_read ON notifications(is_read);

-- Views for common queries
CREATE VIEW medicine_inventory_view AS
SELECT 
    m.medicine_id,
    m.medicine_name,
    m.generic_name,
    c.category_name,
    s.supplier_name,
    m.strength,
    m.formulation,
    m.batch_number,
    m.manufacture_date,
    m.expiry_date,
    m.stock_quantity,
    m.min_stock_level,
    m.reorder_level,
    m.selling_price,
    m.purchase_price,
    m.rack_location,
    CASE 
        WHEN m.stock_quantity <= m.min_stock_level THEN 'Critical'
        WHEN m.stock_quantity <= m.reorder_level THEN 'Low'
        ELSE 'Adequate'
    END as stock_status,
    CASE 
        WHEN m.expiry_date <= DATE_ADD(CURRENT_DATE, INTERVAL 30 DAY) THEN 'Expiring Soon'
        WHEN m.expiry_date < CURRENT_DATE THEN 'Expired'
        ELSE 'Good'
    END as expiry_status,
    DATEDIFF(m.expiry_date, CURRENT_DATE) as days_to_expiry
FROM medicines m
LEFT JOIN categories c ON m.category_id = c.category_id
LEFT JOIN suppliers s ON m.supplier_id = s.supplier_id
WHERE m.is_active = TRUE;

CREATE VIEW sales_summary_view AS
SELECT 
    DATE(s.sale_date) as sale_date,
    COUNT(s.sale_id) as total_sales,
    SUM(s.total_amount) as total_revenue,
    SUM(s.subtotal) as gross_sales,
    SUM(s.discount_amount) as total_discount,
    AVG(s.total_amount) as average_sale
FROM sales s
WHERE s.payment_status = 'Paid'
GROUP BY DATE(s.sale_date);

CREATE VIEW top_selling_medicines_view AS
SELECT 
    m.medicine_name,
    m.generic_name,
    c.category_name,
    SUM(si.quantity) as total_quantity_sold,
    SUM(si.total_price) as total_revenue,
    COUNT(DISTINCT si.sale_id) as number_of_sales
FROM medicines m
JOIN sale_items si ON m.medicine_id = si.medicine_id
JOIN sales s ON si.sale_id = s.sale_id
JOIN categories c ON m.category_id = c.category_id
WHERE s.payment_status = 'Paid'
    AND s.sale_date >= DATE_SUB(CURRENT_DATE, INTERVAL 30 DAY)
GROUP BY m.medicine_id, m.medicine_name, m.generic_name, c.category_name
ORDER BY total_quantity_sold DESC;

-- Stored procedures
DELIMITER //

-- Procedure for adding medicine with stock transaction
CREATE PROCEDURE add_medicine(
    IN p_medicine_name VARCHAR(100),
    IN p_generic_name VARCHAR(100),
    IN p_category_id INT,
    IN p_supplier_id INT,
    IN p_description TEXT,
    IN p_strength VARCHAR(50),
    IN p_formulation VARCHAR(50),
    IN p_batch_number VARCHAR(50),
    IN p_manufacture_date DATE,
    IN p_expiry_date DATE,
    IN p_purchase_price DECIMAL(10,2),
    IN p_selling_price DECIMAL(10,2),
    IN p_initial_stock INT,
    IN p_min_stock_level INT,
    IN p_reorder_level INT,
    IN p_rack_location VARCHAR(50),
    IN p_storage_conditions TEXT,
    IN p_user_id INT
)
BEGIN
    DECLARE v_medicine_id INT;
    
    INSERT INTO medicines (
        medicine_name, generic_name, category_id, supplier_id, description,
        strength, formulation, batch_number, manufacture_date, expiry_date,
        purchase_price, selling_price, stock_quantity, min_stock_level,
        reorder_level, rack_location, storage_conditions
    ) VALUES (
        p_medicine_name, p_generic_name, p_category_id, p_supplier_id, p_description,
        p_strength, p_formulation, p_batch_number, p_manufacture_date, p_expiry_date,
        p_purchase_price, p_selling_price, p_initial_stock, p_min_stock_level,
        p_reorder_level, p_rack_location, p_storage_conditions
    );
    
    SET v_medicine_id = LAST_INSERT_ID();
    
    IF p_initial_stock > 0 THEN
        INSERT INTO stock_transactions (
            medicine_id, transaction_type, quantity, reference_type, user_id, notes
        ) VALUES (
            v_medicine_id, 'Purchase', p_initial_stock, 'Purchase', p_user_id, 'Initial stock'
        );
    END IF;
    
    -- Check for low stock notification
    IF p_initial_stock <= p_min_stock_level THEN
        INSERT INTO notifications (
            user_id, title, message, notification_type, priority
        ) VALUES (
            p_user_id, 
            'Low Stock Alert', 
            CONCAT('Medicine ', p_medicine_name, ' has low stock (', p_initial_stock, ')'),
            'Low Stock', 'High'
        );
    END IF;
    
    -- Check for expiry notification
    IF p_expiry_date <= DATE_ADD(CURRENT_DATE, INTERVAL 30 DAY) THEN
        INSERT INTO notifications (
            user_id, title, message, notification_type, priority
        ) VALUES (
            p_user_id, 
            'Expiry Alert', 
            CONCAT('Medicine ', p_medicine_name, ' expires soon (', p_expiry_date, ')'),
            'Expiry Alert', 'High'
        );
    END IF;
END //

-- Procedure for processing sale
CREATE PROCEDURE process_sale(
    IN p_patient_id INT,
    IN p_user_id INT,
    IN p_items JSON,
    IN p_discount_amount DECIMAL(10,2),
    IN p_payment_method VARCHAR(20),
    IN p_notes TEXT
)
BEGIN
    DECLARE v_sale_id INT;
    DECLARE v_invoice_number VARCHAR(50);
    DECLARE v_subtotal DECIMAL(10,2) DEFAULT 0;
    DECLARE v_tax_amount DECIMAL(10,2) DEFAULT 0;
    DECLARE v_total_amount DECIMAL(10,2) DEFAULT 0;
    DECLARE v_item_count INT DEFAULT 0;
    DECLARE v_medicine_id INT;
    DECLARE v_quantity INT;
    DECLARE v_unit_price DECIMAL(10,2);
    DECLARE v_current_stock INT;
    
    -- Generate invoice number
    SET v_invoice_number = CONCAT('INV', DATE_FORMAT(CURRENT_DATE, '%Y%m%d'), LPAD(FLOOR(RAND() * 10000), 4, '0'));
    
    -- Calculate subtotal and validate stock
    WHILE v_item_count < JSON_LENGTH(p_items) DO
        SET v_medicine_id = JSON_UNQUOTE(JSON_EXTRACT(p_items, CONCAT('$[', v_item_count, '].medicine_id')));
        SET v_quantity = JSON_UNQUOTE(JSON_EXTRACT(p_items, CONCAT('$[', v_item_count, '].quantity')));
        SET v_unit_price = JSON_UNQUOTE(JSON_EXTRACT(p_items, CONCAT('$[', v_item_count, '].unit_price')));
        
        -- Check stock availability
        SELECT stock_quantity INTO v_current_stock FROM medicines WHERE medicine_id = v_medicine_id;
        
        IF v_current_stock < v_quantity THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Insufficient stock for medicine';
        END IF;
        
        SET v_subtotal = v_subtotal + (v_unit_price * v_quantity);
        SET v_item_count = v_item_count + 1;
    END WHILE;
    
    -- Calculate tax (assuming 5% GST)
    SET v_tax_amount = v_subtotal * 0.05;
    SET v_total_amount = v_subtotal + v_tax_amount - p_discount_amount;
    
    -- Create sale record
    INSERT INTO sales (
        invoice_number, patient_id, user_id, subtotal, discount_amount, 
        tax_amount, total_amount, payment_method, notes
    ) VALUES (
        v_invoice_number, p_patient_id, p_user_id, v_subtotal, p_discount_amount,
        v_tax_amount, v_total_amount, p_payment_method, p_notes
    );
    
    SET v_sale_id = LAST_INSERT_ID();
    
    -- Process sale items and update stock
    SET v_item_count = 0;
    WHILE v_item_count < JSON_LENGTH(p_items) DO
        SET v_medicine_id = JSON_UNQUOTE(JSON_EXTRACT(p_items, CONCAT('$[', v_item_count, '].medicine_id')));
        SET v_quantity = JSON_UNQUOTE(JSON_EXTRACT(p_items, CONCAT('$[', v_item_count, '].quantity')));
        SET v_unit_price = JSON_UNQUOTE(JSON_EXTRACT(p_items, CONCAT('$[', v_item_count, '].unit_price')));
        
        -- Get batch number
        SELECT batch_number INTO @batch_number FROM medicines WHERE medicine_id = v_medicine_id;
        
        -- Add sale item
        INSERT INTO sale_items (
            sale_id, medicine_id, quantity, unit_price, total_price, batch_number
        ) VALUES (
            v_sale_id, v_medicine_id, v_quantity, v_unit_price, (v_unit_price * v_quantity), @batch_number
        );
        
        -- Update stock
        UPDATE medicines SET stock_quantity = stock_quantity - v_quantity WHERE medicine_id = v_medicine_id;
        
        -- Add stock transaction
        INSERT INTO stock_transactions (
            medicine_id, transaction_type, quantity, reference_id, reference_type, user_id, notes
        ) VALUES (
            v_medicine_id, 'Sale', -v_quantity, v_sale_id, 'Sale', p_user_id, CONCAT('Sale invoice: ', v_invoice_number)
        );
        
        -- Check for low stock after sale
        SELECT stock_quantity INTO v_current_stock FROM medicines WHERE medicine_id = v_medicine_id;
        SELECT min_stock_level INTO @min_stock_level FROM medicines WHERE medicine_id = v_medicine_id;
        
        IF v_current_stock <= @min_stock_level THEN
            INSERT INTO notifications (
                user_id, title, message, notification_type, priority
            ) VALUES (
                p_user_id, 
                'Low Stock Alert', 
                CONCAT('Medicine stock is low after sale. Current stock: ', v_current_stock),
                'Low Stock', 'High'
            );
        END IF;
        
        SET v_item_count = v_item_count + 1;
    END WHILE;
    
    -- Add billing notification
    INSERT INTO notifications (
        user_id, title, message, notification_type, priority
    ) VALUES (
        p_user_id, 
        'Sale Completed', 
        CONCAT('Sale completed successfully. Invoice: ', v_invoice_number, ', Amount: ₹', v_total_amount),
        'Billing', 'Medium'
    );
    
    SELECT v_sale_id as sale_id, v_invoice_number as invoice_number, v_total_amount as total_amount;
END //

-- Procedure for generating daily summary
CREATE PROCEDURE generate_daily_summary()
BEGIN
    DECLARE v_date DATE DEFAULT CURRENT_DATE;
    DECLARE v_total_sales INT DEFAULT 0;
    DECLARE v_total_revenue DECIMAL(10,2) DEFAULT 0;
    DECLARE v_low_stock_count INT DEFAULT 0;
    DECLARE v_expiry_soon_count INT DEFAULT 0;
    
    -- Get daily sales data
    SELECT COUNT(sale_id), SUM(total_amount) INTO v_total_sales, v_total_revenue
    FROM sales 
    WHERE DATE(sale_date) = v_date AND payment_status = 'Paid';
    
    -- Get low stock count
    SELECT COUNT(*) INTO v_low_stock_count
    FROM medicines 
    WHERE stock_quantity <= min_stock_level AND is_active = TRUE;
    
    -- Get expiry soon count
    SELECT COUNT(*) INTO v_expiry_soon_count
    FROM medicines 
    WHERE expiry_date <= DATE_ADD(CURRENT_DATE, INTERVAL 30 DAY) 
    AND expiry_date >= CURRENT_DATE AND is_active = TRUE;
    
    -- Create summary notification for all users
    INSERT INTO notifications (user_id, title, message, notification_type, priority)
    SELECT 
        user_id,
        'Daily Summary',
        CONCAT(
            'Date: ', v_date, 
            ' | Sales: ', v_total_sales, 
            ' | Revenue: ₹', IFNULL(v_total_revenue, 0),
            ' | Low Stock: ', v_low_stock_count,
            ' | Expiring Soon: ', v_expiry_soon_count
        ),
        'Daily Summary',
        'Low'
    FROM users WHERE is_active = TRUE;
END //

DELIMITER ;

-- Triggers
DELIMITER //

-- Trigger to check expiry and low stock on medicine update
CREATE TRIGGER check_medicine_alerts
BEFORE UPDATE ON medicines
FOR EACH ROW
BEGIN
    -- Check for low stock
    IF NEW.stock_quantity <= NEW.min_stock_level AND OLD.stock_quantity > NEW.min_stock_level THEN
        INSERT INTO notifications (
            user_id, title, message, notification_type, priority
        ) VALUES (
            (SELECT user_id FROM users WHERE is_active = TRUE LIMIT 1),
            'Low Stock Alert',
            CONCAT('Medicine ', NEW.medicine_name, ' has low stock (', NEW.stock_quantity, ')'),
            'Low Stock',
            'High'
        );
    END IF;
    
    -- Check for expiry
    IF NEW.expiry_date <= DATE_ADD(CURRENT_DATE, INTERVAL 30 DAY) THEN
        INSERT INTO notifications (
            user_id, title, message, notification_type, priority
        ) VALUES (
            (SELECT user_id FROM users WHERE is_active = TRUE LIMIT 1),
            'Expiry Alert',
            CONCAT('Medicine ', NEW.medicine_name, ' expires on ', NEW.expiry_date),
            'Expiry Alert',
            'High'
        );
    END IF;
END //

DELIMITER ;

-- Insert initial data
INSERT INTO categories (category_name, description) VALUES
('Antibiotics', 'Antibacterial and antimicrobial medications'),
('Pain Relief', 'Analgesics and pain management'),
('Vitamins', 'Vitamin supplements and minerals'),
('Diabetes', 'Diabetes management medications'),
('Cardiovascular', 'Heart and blood pressure medications'),
('Respiratory', 'Asthma and respiratory medications'),
('Gastrointestinal', 'Digestive system medications'),
('Dermatology', 'Skin care medications'),
('Pediatric', 'Children medications'),
('General', 'General pharmacy items');

INSERT INTO suppliers (supplier_name, contact_person, phone, email, address, gst_number) VALUES
('MediCorp Pharmaceuticals', 'Rajesh Kumar', '9876543210', 'rajesh@medicorp.com', '123 Medical Street, Mumbai', '27AAAPL1234C1ZV'),
('PharmaPlus Distributors', 'Anita Sharma', '9876543211', 'anita@pharmaplus.in', '456 Hospital Road, Delhi', '27AABFP1234C1ZV'),
('HealthCare Suppliers', 'Vikram Singh', '9876543212', 'vikram@healthcare.com', '789 Pharmacy Lane, Bangalore', '27AACH1234C1ZV'),
('Global Pharma Ltd', 'Priya Patel', '9876543213', 'priya@globalpharma.com', '321 Medicine Avenue, Chennai', '27AAFC1234C1ZV');

INSERT INTO users (username, password, full_name, email, phone, role) VALUES
('admin', 'admin123', 'System Administrator', 'admin@pharmacy.com', '9876543210', 'admin'),
('pharmacist1', 'pharma123', 'Dr. Amit Sharma', 'amit@pharmacy.com', '9876543211', 'pharmacist'),
('pharmacist2', 'pharma123', 'Dr. Neha Gupta', 'neha@pharmacy.com', '9876543212', 'pharmacist');

INSERT INTO medicines (
    medicine_name, generic_name, category_id, supplier_id, description,
    strength, formulation, batch_number, manufacture_date, expiry_date,
    purchase_price, selling_price, stock_quantity, min_stock_level, reorder_level, rack_location
) VALUES
('Dolo 650', 'Paracetamol', 2, 1, 'Fever and pain relief tablet', '650mg', 'Tablet', 'DL6502024001', '2024-01-15', '2027-01-14', 12.00, 15.50, 150, 20, 50, 'A1-1'),
('Crocin Advance', 'Paracetamol', 2, 2, 'Advanced pain relief', '500mg', 'Tablet', 'CR5002024001', '2024-02-01', '2027-01-31', 9.50, 12.00, 200, 25, 60, 'A1-2'),
('Azithral 500', 'Azithromycin', 1, 3, 'Antibiotic for infections', '500mg', 'Tablet', 'AZ5002024001', '2024-01-20', '2026-12-31', 75.00, 85.00, 80, 15, 40, 'B2-1'),
('Augmentin 625', 'Amoxicillin', 1, 4, 'Broad spectrum antibiotic', '625mg', 'Tablet', 'AU6252024001', '2024-01-10', '2026-12-20', 105.00, 120.00, 60, 10, 30, 'B2-2'),
('Pantop D', 'Pantoprazole', 7, 1, 'Acidity and GERD treatment', '40mg', 'Capsule', 'PN402024001', '2024-02-05', '2027-01-31', 80.00, 95.00, 100, 20, 45, 'C1-1'),
('Metformin 500', 'Metformin', 4, 2, 'Diabetes management', '500mg', 'Tablet', 'MT5002024001', '2024-01-25', '2027-01-24', 15.00, 22.00, 250, 30, 70, 'D1-1'),
('Amlodipine 5mg', 'Amlodipine', 5, 3, 'Blood pressure control', '5mg', 'Tablet', 'AM52024001', '2024-01-15', '2027-01-14', 25.00, 35.00, 180, 25, 55, 'E1-1'),
('Asthalin Inhaler', 'Salbutamol', 6, 4, 'Asthma relief inhaler', '100mcg', 'Inhaler', 'AS1002024001', '2024-02-10', '2026-11-30', 120.00, 145.00, 40, 8, 20, 'F1-1'),
('Betnovate Cream', 'Betamethasone', 8, 1, 'Skin inflammation treatment', '0.05%', 'Cream', 'BN052024001', '2024-01-30', '2026-12-29', 45.00, 55.00, 75, 12, 35, 'G1-1'),
('Calpol Syrup', 'Paracetamol', 9, 2, 'Children fever medicine', '125mg/5ml', 'Syrup', 'CP1252024001', '2024-02-01', '2026-07-31', 35.00, 42.00, 90, 15, 40, 'H1-1');

INSERT INTO patients (patient_name, age, gender, phone, email, address, medical_history, allergies, emergency_contact) VALUES
('Ramesh Kumar', 45, 'Male', '9876543210', 'ramesh@email.com', '123 Main Street, Delhi', 'Hypertension, Diabetes', 'Penicillin', '9876543211'),
('Sunita Devi', 32, 'Female', '9876543212', 'sunita@email.com', '456 Park Avenue, Mumbai', 'No major illnesses', 'None', '9876543213'),
('Amit Sharma', 28, 'Male', '9876543214', 'amit@email.com', '789 Cross Road, Bangalore', 'Asthma', 'Dust allergy', '9876543215'),
('Priya Patel', 35, 'Female', '9876543216', 'priya@email.com', '321 Circle Lane, Chennai', 'Thyroid issues', 'Iodine', '9876543217'),
('Vikram Singh', 52, 'Male', '9876543218', 'vikram@email.com', '654 Market Street, Kolkata', 'Heart disease', 'Aspirin', '9876543219');

INSERT INTO system_settings (setting_key, setting_value, description) VALUES
('pharmacy_name', 'MediCare Pharmacy', 'Pharmacy business name'),
('pharmacy_address', '123 Medical Complex, Delhi - 110001', 'Pharmacy address'),
('pharmacy_phone', '9876543210', 'Pharmacy contact number'),
('pharmacy_email', 'info@medicarepharmacy.com', 'Pharmacy email address'),
('gst_number', '27AAAPL1234C1ZV', 'Pharmacy GST registration number'),
('drug_license_number', 'DL/2024/DEL/12345', 'Drug license number'),
('default_tax_rate', '5', 'Default tax rate percentage'),
('low_stock_alert_days', '30', 'Days before expiry to show alert'),
('currency_symbol', '₹', 'Currency symbol for display'),
('receipt_footer', 'Thank you for your visit! Please come again.', 'Footer text for receipts');

-- Create initial notifications
INSERT INTO notifications (user_id, title, message, notification_type, priority) VALUES
(2, 'Welcome to Pharmacy Management System', 'System is ready for use. Please verify inventory and update stock levels.', 'System', 'Low'),
(2, 'Low Stock Alert', 'Azithral 500 has low stock (80 units remaining)', 'Low Stock', 'High'),
(2, 'Expiry Alert', 'Asthalin Inhaler expires in less than 30 days', 'Expiry Alert', 'High');
