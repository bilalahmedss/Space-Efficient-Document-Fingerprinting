from flask import Flask, request, render_template, redirect, url_for
import subprocess
import os

app = Flask(__name__)
UPLOAD_FOLDER = 'uploads'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')

@app.route('/check-dataset', methods=['POST'])
def check_dataset():
    file = request.files['file']
    filepath = os.path.join(UPLOAD_FOLDER, file.filename)
    file.save(filepath)
    
    # Check if the executable exists
    executable_path = './build/DocumentFingerprinting'
    if not os.path.exists(executable_path):
        return "Error: DocumentFingerprinting executable not found. Please build the project and try again.", 500

    # Call your C++ tool
    result = subprocess.check_output([
        executable_path, '--check', filepath, 'dataset_cbf'
    ], universal_newlines=True)
    result_lines = result.strip().splitlines()
    return render_template('result.html', result_lines=result_lines)

@app.route('/compare-files', methods=['POST'])
def compare_files():
    file1 = request.files['file1']
    file2 = request.files['file2']
    path1 = os.path.join(UPLOAD_FOLDER, file1.filename)
    path2 = os.path.join(UPLOAD_FOLDER, file2.filename)
    file1.save(path1)
    file2.save(path2)
    result = subprocess.check_output([
        './build/DocumentFingerprinting', path1, path2
    ], universal_newlines=True)
    result_lines = result.strip().splitlines()
    return render_template('result.html', result_lines=result_lines)

if __name__ == '__main__':

    app.run(debug=True)