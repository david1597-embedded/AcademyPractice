from flask import Flask, jsonify, request
from flask_cors import CORS
import mysql.connector
from mysql.connector import Error
import sys

app = Flask(__name__)
CORS(app)  # ��� ��������Ʈ�� CORS ��� (Access-Control-Allow-Origin: *)

# MySQL �����ͺ��̽� ���� ����
def get_db_connection():
    try:
        connection = mysql.connector.connect(
            host='localhost',
            database='User',
            user='root',
            password='1234'
        )
        if connection.is_connected():
            print("Successfully connected to the MySQL database 'User'")
            return connection
    except Error as e:
        print(f"Error connecting to MySQL: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error during database connection: {e}")
        sys.exit(1)

# ��� ����� ��ȸ
@app.route('/users', methods=['GET'])
def get_users():
    connection = get_db_connection()
    if connection is None:
        print("Failed to get database connection")
        sys.exit(1)
    
    try:
        cursor = connection.cursor(dictionary=True)
        cursor.execute("SELECT id, name, password FROM data")
        users = cursor.fetchall()
        return jsonify(users)
    except Error as e:
        print(f"Database error during query execution: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error during query execution: {e}")
        sys.exit(1)
    finally:
        if 'cursor' in locals():
            cursor.close()
        if connection.is_connected():
            connection.close()
            print("Database connection closed")

# �α��� ó��
@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    id = data.get('id')
    password = data.get('password')

    if not id or not password:
        return jsonify({"error": "Username and password are required"}), 400

    connection = get_db_connection()
    if connection is None:
        print("Failed to get database connection")
        sys.exit(1)
    
    try:
        cursor = connection.cursor(dictionary=True)
        cursor.execute("SELECT id, name, password FROM data WHERE id = %s", (id,))
        user = cursor.fetchone()
        
        if user:
            if user['password'] == password:
                return jsonify({
                    "message": "Login successful",
                    "id": user['id'],
                    "name": user['name']
                }), 200
            else:
                return jsonify({"error": "Invalid password"}), 401
        else:
            return jsonify({"error": "Username not found"}), 404
    except Error as e:
        print(f"Database error during login: " + str(e))
        return jsonify({"error": "Database error: " + str(e)}), 500
    except Exception as e:
        print(f"Unexpected error during login: " + str(e))
        return jsonify({"error": "Unexpected error: " + str(e)}), 500
    finally:
        if 'cursor' in locals():
            cursor.close()
        if connection.is_connected():
            connection.close()
            print("Database connection closed")

if __name__ == '__main__':
    connection = get_db_connection()
    if connection:
        connection.close()
        print("Initial database connection test successful")
    try:
        app.run(host='0.0.0.0',debug=True, port=5001)
    except Exception as e:
        print(f"Error starting Flask application: {e}")
        sys.exit(1)
