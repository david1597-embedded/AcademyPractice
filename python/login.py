from flask import Flask, jsonify, request
from flask_cors import CORS
import mysql.connector
from mysql.connector import Error
import sys

app = Flask(__name__)
CORS(app)  # ��� ��������Ʈ�� CORS ��� (Access-Control-Allow-Origin: *)

# MySQL �����ͺ��̽� ���� ����
def get_db_connection(db_name):
    try:
        connection = mysql.connector.connect(
            host='localhost',
            database=db_name,
            user='root',
            password='1234'
        )
        if connection.is_connected():
            print("Successfully connected to the MySQL database ")
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
    connection = get_db_connection('User')
    if connection is None:
        print("Failed to get database connection")
        sys.exit(1)
    
    try:
        cursor = connection.cursor(dictionary=True)
        cursor.execute("SELECT id, name, password ,num FROM data")
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

    connection = get_db_connection("User")
    if connection is None:
        print("Failed to get database connection")
        sys.exit(1)
    
    try:
        cursor = connection.cursor(dictionary=True)
        cursor.execute("SELECT id, name, password, num FROM data WHERE id = %s", (id,))
        user = cursor.fetchone()
        
        if user:
            if user['password'] == password:
                return jsonify({
                    "message": "Login successful",
                    "id": user['id'],
                    "name": user['name'],
                    "num" :user['num']
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


@app.route('/fetch_data', methods=['POST'])
def fetch_data():
    data = request.get_json()
    num = data.get('num')

    if not num:
        return jsonify({"error": "num is required"}), 400

    table_name = f"park{num}"
    result = {
        "DHT11": {},
        "Park": {}
    }

    dht11_conn = get_db_connection('DHT11')
    if dht11_conn is None:
        return jsonify({"error": "Failed to connect to DHT11 database"}), 500

    try:
        cursor = dht11_conn.cursor(dictionary=True)
        # id �ִ밪���� �ֽ� ������ ��ȸ
        cursor.execute(f"SELECT * FROM {table_name} WHERE id = (SELECT MAX(id) FROM {table_name})")
        result["DHT11"] = cursor.fetchone() or {}
    except Error as e:
        print(f"Database error in DHT11: {e}")
        return jsonify({"error": f"DHT11 database error: {e}"}), 500
    finally:
        if 'cursor' in locals():
            cursor.close()
        if dht11_conn.is_connected():
            dht11_conn.close()

    park_conn = get_db_connection('Park')
    if park_conn is None:
        return jsonify({"error": "Failed to connect to Park database"}), 500

    try:
        cursor = park_conn.cursor(dictionary=True)
        # id �ִ밪���� �ֽ� ������ ��ȸ
        cursor.execute(f"SELECT * FROM {table_name} WHERE id = (SELECT MAX(id) FROM {table_name})")
        result["Park"] = cursor.fetchone() or {}
    except Error as e:
        print(f"Database error in Park: {e}")
        return jsonify({"error": f"Park database error: {e}"}), 500
    finally:
        if 'cursor' in locals():
            cursor.close()
        if park_conn.is_connected():
            park_conn.close()

    return jsonify({"message": "Data fetched successfully", "data": result}), 200






if __name__ == '__main__':
    connection = get_db_connection('User')
    if connection:
        connection.close()
        print("Initial database connection test successful")
    try:
        app.run(host='0.0.0.0',debug=True, port=5001)
    except Exception as e:
        print(f"Error starting Flask application: {e}")
        sys.exit(1)
