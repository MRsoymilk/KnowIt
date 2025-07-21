from contextlib import contextmanager
from flask import Flask, jsonify, request, send_file
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from bs4 import BeautifulSoup
from urllib.parse import urljoin
import os
import requests
import logging
import json
from retrying import retry
from datetime import datetime
from werkzeug.utils import secure_filename
import cv2
from PIL import Image
import numpy as np
import logging
from scipy.signal import find_peaks
import time
import shutil

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

app = Flask(__name__)

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

COOKIE_FILE = "sdbs_cookies.json"

@contextmanager
def get_driver():
    options = Options()
    options.headless = True
    options.add_argument('--no-sandbox')
    options.add_argument('--disable-dev-shm-usage')
    options.add_argument('user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36')
    driver = webdriver.Chrome(options=options)
    try:
        yield driver
    finally:
        driver.quit()

def get_text_by_id(soup, id_):
    tag = soup.find(id=id_)
    return tag.get_text(strip=True) if tag else ''

def save_cookies(driver, path=COOKIE_FILE):
    try:
        cookies = driver.get_cookies()
        with open(path, "w", encoding="utf-8") as f:
            json.dump(cookies, f, indent=4)
        logging.info("Cookies saved to %s", path)
        return True
    except Exception as e:
        logging.error("Failed to save cookies: %s", str(e))
        return False

def load_cookies(driver, path=COOKIE_FILE):
    try:
        if not os.path.exists(path):
            logging.warning("Cookie file %s does not exist", path)
            return False
        with open(path, "r", encoding="utf-8") as f:
            cookies = json.load(f)
        for cookie in cookies:
            driver.add_cookie(cookie)
        logging.info("Cookies loaded from %s", path)
        return True
    except Exception as e:
        logging.error("Failed to load cookies: %s", str(e))
        return False

def accept_agreement(driver):
    try:
        driver.get('https://sdbs.db.aist.go.jp/')
        if load_cookies(driver):
            driver.refresh()
            wait = WebDriverWait(driver, 10)
            try:
                wait.until(EC.presence_of_element_located(
                    (By.LINK_TEXT, 'Spectral Search')
                ))
                logging.info("Successfully loaded cookies and accessed main page")
                return True
            except Exception as e:
                logging.warning("Loaded cookies but failed to access main page: %s", str(e))
        
        logging.info("Agreement Page Title: %s", driver.title)
        with open("agreement_page.html", "w", encoding="utf-8") as f:
            f.write(driver.page_source)
    
        wait = WebDriverWait(driver, 10)
        try:
            accept_button = wait.until(EC.element_to_be_clickable(
                (By.ID, 'BodyContentPlaceHolder_DisclaimeraAccept')
            ))
            driver.execute_script("arguments[0].scrollIntoView(true);", accept_button)
            accept_button.click()
            logging.info("Clicked accept button")
            wait.until(EC.presence_of_element_located(
                (By.LINK_TEXT, 'Spectral Search')
            ))
            driver.execute_script("return document.readyState === 'complete';")
        except Exception as e:
            logging.warning("Failed to click accept button, trying JavaScript click: %s", str(e))
            driver.execute_script("document.getElementById('BodyContentPlaceHolder_DisclaimeraAccept').click();")
            wait.until(EC.presence_of_element_located(
                (By.LINK_TEXT, 'Spectral Search')
            ))
            driver.execute_script("return document.readyState === 'complete';")
        
        save_cookies(driver)
        return True
    except Exception as e:
        logging.error("Failed to accept agreement: %s", str(e))
        return False

def is_url_valid(url):
    try:
        response = requests.head(url, timeout=5)
        return response.status_code == 200
    except requests.RequestException as e:
        logging.error("URL validation failed: %s", str(e))
        return False

@retry(stop_max_attempt_number=3, wait_fixed=2000)
def doAnalysisRamanSpectralView(target_url):
    if not is_url_valid(target_url):
        return {"status": False, "error": "Invalid or unreachable URL"}
    
    with get_driver() as driver:
        try:
            if not accept_agreement(driver):
                return {"status": False, "error": "Failed to accept agreement"}

            wait = WebDriverWait(driver, 10)
            driver.get(target_url)
            logging.info("Target Page Title: %s", driver.title)
            logging.info("Current URL: %s", driver.current_url)

            if driver.title == "Runtime Error":
                return {"status": False, "error": "Failed to load target page: Runtime Error"}

            wait.until(lambda d: d.execute_script("return document.readyState === 'complete';"))
            wait.until(EC.presence_of_element_located(
                (By.ID, 'CtlMasterSideMenu_CompoundName')
            ))

            with open("RamanSpectralView.html", "w", encoding="utf-8") as f:
                f.write(driver.page_source)
            
            with open("RamanSpectralView.html", "r", encoding="utf-8") as f:
                content = f.read()
                if len(content) < 100:
                    return {"status": False, "error": "Saved page content is too short or invalid"}

            soup = BeautifulSoup(driver.page_source, 'html.parser')
            data = {}
            data['Compound Name'] = get_text_by_id(soup, 'CtlMasterSideMenu_CompoundName')
            data['Molecular Formula'] = get_text_by_id(soup, 'CtlMasterSideMenu_MolecularFormula')
            data['Molecular Weight'] = get_text_by_id(soup, 'CtlMasterSideMenu_MolecularWeight')
            data['CAS Registry No.'] = get_text_by_id(soup, 'CtlMasterSideMenu_CasRegistryNo')

            pnl_main = soup.find('div', id='pnlMain')
            if pnl_main:
                img = pnl_main.find('img')
                if img and img.has_attr('src'):
                    data['Image URL'] = urljoin(target_url, img['src'])

            if not any(data.values()):
                return {"status": False, "error": "No data extracted from page"}

            logging.info("Extracted data: %s", data)
            return {"status": True, "data": data}
        except Exception as e:
            logging.error("Error in doAnalysisRamanSpectralView: %s", str(e))
            return {"status": False, "error": str(e)}

@retry(stop_max_attempt_number=3, wait_fixed=2000)
def doAnalysisCompoundView(target_url):
    if not is_url_valid(target_url):
        return {"status": False, "error": "Invalid or unreachable URL"}
    
    with get_driver() as driver:
        try:
            if not accept_agreement(driver):
                return {"status": False, "error": "Failed to accept agreement"}

            wait = WebDriverWait(driver, 10)
            driver.get(target_url)
            logging.info("Target Page Title: %s", driver.title)
            logging.info("Current URL: %s", driver.current_url)

            if driver.title == "Runtime Error":
                return {"status": False, "error": "Failed to load target page: Runtime Error"}

            wait.until(lambda d: d.execute_script("return document.readyState === 'complete';"))
            wait.until(EC.presence_of_element_located(
                (By.ID, 'BodyContentPlaceHolder_CASRegistryNos')
            ))

            with open("CompoundView.html", "w", encoding="utf-8") as f:
                f.write(driver.page_source)
            
            with open("CompoundView.html", "r", encoding="utf-8") as f:
                content = f.read()
                if len(content) < 100:
                    return {"status": False, "error": "Saved page content is too short or invalid"}

            soup = BeautifulSoup(driver.page_source, 'html.parser')
            data = {}
            data['CAS Registry No.'] = get_text_by_id(soup, 'BodyContentPlaceHolder_CASRegistryNos')
            data['Molecular Formula'] = get_text_by_id(soup, 'BodyContentPlaceHolder_MolecularFormula')
            data['Molecular Weight'] = get_text_by_id(soup, 'BodyContentPlaceHolder_MolecularWeight')
            element = soup.find(id='BodyContentPlaceHolder_CompoundNames')
            if element:
                data['Compound Name'] = element.decode_contents().replace("<br>", ", ").replace("<br/>", ", ").strip()
            else:
                data['Compound Name'] = None 

            img = soup.find('img', id='BodyContentPlaceHolder_MolecularImage')
            if img and img.has_attr('src'):
                data['Image URL'] = urljoin(target_url, img['src'])

            if not any(data.values()):
                return {"status": False, "error": "No data extracted from page"}

            logging.info("Extracted data: %s", data)
            return {"status": True, "data": data}
        except Exception as e:
            logging.error("Error in doAnalysisCompoundView: %s", str(e))
            return {"status": False, "error": str(e)}

@app.route("/html/analysis", methods=["POST"])
def html_analysis():
    data = request.get_json()
    if not data:
        return jsonify({"status": False, "message": "Invalid JSON data"}), 400

    if 'url_raman_spectral_view' in data:
        url = data['url_raman_spectral_view']
        if not url.startswith('https://sdbs.db.aist.go.jp/') or 'sdbsno' not in url:
            return jsonify({"status": False, "message": "Invalid URL: Must start with https://sdbs.db.aist.go.jp/ and include sdbsno parameter"}), 400
        result = doAnalysisRamanSpectralView(url)
        return jsonify(result)

    if 'url_compound_view' in data:
        url = data['url_compound_view']
        if not url.startswith('https://sdbs.db.aist.go.jp/') or 'sdbsno' not in url:
            return jsonify({"status": False, "message": "Invalid URL: Must start with https://sdbs.db.aist.go.jp/ and include sdbsno parameter"}), 400
        result = doAnalysisCompoundView(url)
        return jsonify(result)
    
    return jsonify({"status": False, "message": "Missing URL parameter (url_raman_spectral_view or url_compound_view)"}), 400

@app.route("/dataset/check", methods=["POST"])
def dataset_check():
    data = request.get_json()
    db_id = data.get("ID")
    if not db_id:
        return jsonify({"status": False, "message": "ID is missing"}), 400

    base_dir = os.path.join("datasets", db_id)

    if os.path.exists(base_dir):
        return jsonify({"status": False, "message": "ID already exists"}), 409

    try:
        os.makedirs(base_dir)
        return jsonify({"status": True, "message": "Directory created"})
    except Exception as e:
        logging.error("Error in dataset_check: %s", str(e))
        return jsonify({"status": False, "message": str(e)}), 500

@app.route("/dataset/set", methods=["POST"])
def dataset_set():
    data = request.get_json()
    if not data or "ID" not in data:
        return jsonify({"status": False, "message": "Missing 'ID' in JSON"}), 400

    dataset_id = data["ID"]
    uuid = data["uuid"]

    dir_uuid = os.path.join("uploads", uuid)
    save_path = os.path.join("datasets", dataset_id)
    os.makedirs(save_path, exist_ok=True)

    file_path = os.path.join(save_path, "data.json")
    original_dir = os.path.join(save_path, "original")
    os.makedirs(original_dir, exist_ok=True)

    try:
        # 写入 data.json
        with open(file_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        # 将 uploads/uuid/ 下的文件和子目录移动到 original/ 下
        if os.path.exists(dir_uuid):
            for item in os.listdir(dir_uuid):
                src = os.path.join(dir_uuid, item)
                dst = os.path.join(original_dir, item)
                shutil.move(src, dst)

            # 清理空的 uploads/uuid/ 目录
            os.rmdir(dir_uuid)

        return jsonify({"status": True, "message": "Saved successfully"})
    except Exception as e:
        print(f"Error saving: {e}")
        return jsonify({"status": False, "message": f"Error saving: {e}"}), 500

@app.route("/dataset/get", methods=["POST"])
def dataset_get():
    data = request.get_json()
    if not data:
        logging.error("Invalid JSON data in request")
        return jsonify({"status": False, "message": "Invalid JSON data"}), 400

    dataset_base_dir = "datasets"

    if "ID" in data:
        dataset_id = data["ID"]
        dataset_dir = os.path.join(dataset_base_dir, dataset_id)
        data_file = os.path.join(dataset_dir, "data.json")

        # 检查目录是否存在
        if not os.path.exists(dataset_dir):
            logging.error(f"Dataset directory not found: {dataset_dir}")
            return jsonify({"status": False, "message": f"Dataset ID {dataset_id} not found"}), 404

        # 检查 data.json 文件是否存在
        if not os.path.exists(data_file):
            logging.error(f"data.json not found in directory: {dataset_dir}")
            return jsonify({"status": False, "message": f"data.json not found for ID {dataset_id}"}), 404

        try:
            # 读取 data.json 文件
            with open(data_file, "r", encoding="utf-8") as f:
                data_content = json.load(f)
            logging.info(f"Successfully loaded data.json for ID: {dataset_id}")
            return jsonify({"status": True, "data": data_content})
        except json.JSONDecodeError as e:
            logging.error(f"Failed to parse data.json for ID {dataset_id}: {str(e)}")
            return jsonify({"status": False, "message": f"Invalid JSON in data.json: {str(e)}"}), 400
        except Exception as e:
            logging.error(f"Error reading data.json for ID {dataset_id}: {str(e)}")
            return jsonify({"status": False, "message": f"Error reading data.json: {str(e)}"}), 500
    else:
        # 获取 dataset 目录下所有子目录
        try:
            if not os.path.exists(dataset_base_dir):
                logging.error(f"Dataset base directory not found: {dataset_base_dir}")
                return jsonify({"status": False, "message": "Dataset directory not found"}), 404

            # 列出所有子目录
            directories = [
                d for d in os.listdir(dataset_base_dir)
                if os.path.isdir(os.path.join(dataset_base_dir, d))
            ]
            logging.info(f"Found {len(directories)} datasets: {directories}")
            return jsonify({"status": True, "data": directories})
        except Exception as e:
            logging.error(f"Error listing datasets: {str(e)}")
            return jsonify({"status": False, "message": f"Error listing datasets: {str(e)}"}), 500

@app.route("/dataset/del", methods=["POST"])
def dataset_delete():
    data = request.get_json()
    dataset_id = data["ID"]

    dataset_base_dir = "datasets"
    trash_base_dir = "trash"

    dataset_dir = os.path.join(dataset_base_dir, dataset_id)

    if not os.path.exists(dataset_dir):
        print(f"Dataset {dataset_id} not found")
        return jsonify({"status": "error", "message": "Dataset not found"}), 404

    # 创建 trash 目录（如果不存在）
    os.makedirs(trash_base_dir, exist_ok=True)

    # 获取当前时间戳
    timestamp = time.strftime("%Y%m%d_%H%M%S")

    # 构造目标目录名：datasetID_时间戳
    trash_dir_name = f"{dataset_id}_{timestamp}"
    trash_dir_path = os.path.join(trash_base_dir, trash_dir_name)

    try:
        shutil.move(dataset_dir, trash_dir_path)
        return jsonify({"status": "success", "message": f"Dataset moved to trash as {trash_dir_name}"}), 200
    except Exception as e:
        print(f"error: {str(e)}")
        return jsonify({"status": "error", "message": str(e)}), 500
    

@app.route("/ping")
def ping():
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return jsonify({"pong": now})

# 上传路径配置
UPLOAD_FOLDER = './uploads'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

# 允许的扩展名
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg', 'bmp', 'gif'}

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route("/upload/img", methods=["POST"])
def upload_file():
    # 获取文件
    file = request.files.get('file')
    if not file or file.filename == '':
        return jsonify({'error': 'No file uploaded'}), 400

    # 校验格式
    if not allowed_file(file.filename):
        return jsonify({'error': 'Invalid file type'}), 400

    # 获取 UUID 字段
    uuid = request.form.get("UUID", "")
    if not uuid:
        return jsonify({'error': 'UUID is required'}), 400

    # 创建 UUID 对应的子目录
    uuid_dir = os.path.join(app.config['UPLOAD_FOLDER'], uuid)
    os.makedirs(uuid_dir, exist_ok=True)

    # 保存文件到 UUID 目录
    filename = secure_filename(file.filename)
    save_path = os.path.join(uuid_dir, filename)
    file.save(save_path)

    return jsonify({
        'status': True,
        'filename': filename,
        'uuid': uuid,
        'path': f"{uuid}/{filename}"
    }), 200


@app.route("/load/img/<dataset_id>/original/<filename>")
def load_image(dataset_id, filename):
    path = os.path.join("datasets", dataset_id, "original", filename)

    if not os.path.isfile(path):
        return jsonify({"status": False, "message": f"File not found: {path}"}), 404

    try:
        return send_file(path, mimetype="image/*")
    except Exception as e:
        print(f"Failed to load image: {e}")
        return jsonify({"status": False, "message": f"Failed to load image: {e}"}), 500

@app.route("/cv/find/crop", methods=["POST"])
def cv_find_crop():
    uuid = request.json.get("UUID")
    if not uuid:
        return jsonify({'error': 'Missing uuid'}), 400

    uuid_path = os.path.join(app.config['UPLOAD_FOLDER'], uuid)
    print(uuid_path)
    if not os.path.isdir(uuid_path):
        return jsonify({'error': 'UUID directory not found'}), 404

    # 寻找第一个图像文件
    files = [f for f in os.listdir(uuid_path) if allowed_file(f)]
    if not files:
        return jsonify({'error': 'No valid image in folder'}), 400

    img_path = os.path.join(uuid_path, files[0])
    print(img_path)
    pil_image = Image.open(img_path).convert("RGB")
    img = cv2.cvtColor(np.array(pil_image), cv2.COLOR_RGB2BGR)
    if img is None:
        return jsonify({'error': 'Failed to load image'}), 500

    # 图像处理
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)
    edges = cv2.Canny(binary, 50, 150, apertureSize=3)

    # 霍夫变换
    lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 100, minLineLength=200, maxLineGap=5)
    if lines is None:
        return jsonify({'error': 'No lines detected'}), 400

    horizontal = []
    vertical = []

    for line in lines:
        x1, y1, x2, y2 = line[0]
        if abs(y1 - y2) < 10:
            horizontal.append((x1, y1, x2, y2))
        elif abs(x1 - x2) < 10:
            vertical.append((x1, y1, x2, y2))

    if len(horizontal) < 2 or len(vertical) < 2:
        return jsonify({'error': 'Not enough horizontal or vertical lines'}), 400

    top_line = min(horizontal, key=lambda l: l[1])
    bottom_line = max(horizontal, key=lambda l: l[1])
    left_line = min(vertical, key=lambda l: l[0])
    right_line = max(vertical, key=lambda l: l[0])

    def intersect(h_line, v_line):
        return (v_line[0], h_line[1])

    A = intersect(top_line, left_line)
    B = intersect(top_line, right_line)
    C = intersect(bottom_line, left_line)
    D = intersect(bottom_line, right_line)

    # 绘制点和文字
    for name, point in zip(['A', 'B', 'C', 'D'], [A, B, C, D]):
        cv2.circle(img, point, 5, (0, 0, 255), -1)
        cv2.putText(img, name, (point[0] - 20, point[1] + 20),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    result_path = os.path.join(uuid_path, "result.png")
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    # 转为 Pillow 图像对象
    pil_image = Image.fromarray(img_rgb)
    pil_image.save(result_path)

    def to_native_point(p):
        return [int(p[0]), int(p[1])]
    print(A, B, C, D)
    return jsonify({
        "status": True,
        "uuid": uuid,
        "points": {
            "A": to_native_point(A),
            "B": to_native_point(B),
            "C": to_native_point(C),
            "D": to_native_point(D)
        }
    })


@app.route("/cv/find/curve", methods=["POST"])
def cv_find_curve():
    uuid = request.json.get("UUID")
    if not uuid:
        print("Missing uuid")
        return jsonify({'error': 'Missing uuid'}), 400

    uuid_path = os.path.join(app.config['UPLOAD_FOLDER'], uuid)
    if not os.path.isdir(uuid_path):
        print("UUID directory not found")
        return jsonify({'error': 'UUID directory not found'}), 404

    file = request.json.get("file")

    # Always load the original image to avoid cumulative modifications
    img_path = os.path.join(uuid_path, file)
    pil_image = Image.open(img_path).convert("RGB")
    img = cv2.cvtColor(np.array(pil_image), cv2.COLOR_RGB2BGR)

    # 标定点
    start = request.json.get("start")
    end_4000 = request.json.get("4000")
    end_0 = request.json.get("0")
    middle_2000 = request.json.get("2000")  # 可选

    if not start or not end_4000 or not end_0:
        print("Missing one of: start, 4000, 0")
        return jsonify({'error': 'Missing one of: start, 4000, 0'}), 400

    x_start = int(start["x"])
    x_end = int(end_0["x"])
    y_start = int(start["y"])
    y_end = int(end_0["y"])
    x_middle = int(middle_2000["x"]) if middle_2000 else None

    # Load previous coordinates for comparison (if available)
    coord_path = os.path.join(uuid_path, "last_coords.json")
    prev_coords = {}
    if os.path.exists(coord_path):
        with open(coord_path, 'r') as f:
            prev_coords = json.load(f)

    # Log input coordinates and compare with previous
    print(f"Current coordinates: x_start={x_start}, x_end={x_end}, y_start={y_start}, y_end={y_end}, x_middle={x_middle}")
    print(f"Previous coordinates: {prev_coords.get('coords', 'None')}")

    # Save current coordinates for next request
    coords = {'x_start': x_start, 'x_end': x_end, 'y_start': y_start, 'y_end': y_end, 'x_middle': x_middle}
    with open(coord_path, 'w') as f:
        json.dump({'coords': coords}, f)

    # 边界保护
    height, width = img.shape[:2]
    x_start = max(0, min(x_start, width - 1))
    x_end = max(0, min(x_end, width - 1))
    y_start = max(0, min(y_start, height - 1))
    y_end = max(0, min(y_end, height - 1))
    if x_middle is not None:
        x_middle = max(0, min(x_middle, width - 1))

    # 防止无效范围
    if x_end <= x_start or y_end == y_start:
        print("Invalid calibration range")
        return jsonify({'error': 'Invalid calibration range'}), 400
    if x_middle is not None and (x_middle <= x_start or x_middle >= x_end):
        print("Invalid middle_2000 x-coordinate")
        return jsonify({'error': 'Invalid middle_2000 x-coordinate'}), 400

    # 剪裁图像区域（向内缩一圈）
    crop_margin = 1
    crop_x_start = x_start + crop_margin
    crop_x_end = x_end - crop_margin
    crop_y_start = min(y_start, y_end) + crop_margin
    crop_y_end = max(y_start, y_end) - crop_margin

    if crop_x_end <= crop_x_start or crop_y_end <= crop_y_start:
        print("Crop range too small after margin")
        return jsonify({"error": "Crop range too small after margin"}), 400

    # 确保裁剪范围在图像内
    crop_x_start = max(0, min(crop_x_start, width - 1))
    crop_x_end = max(0, min(crop_x_end, width - 1))
    crop_y_start = max(0, min(crop_y_start, height - 1))
    crop_y_end = max(0, min(crop_y_end, height - 1))

    # Log cropping coordinates
    print(f"Crop coordinates: crop_x_start={crop_x_start}, crop_x_end={crop_x_end}, crop_y_start={crop_y_start}, crop_y_end={crop_y_end}")

    # 保存裁剪区域
    cropped = img[crop_y_start:crop_y_end + 1, crop_x_start:crop_x_end + 1]
    cv2.imwrite(os.path.join(uuid_path, "cropped_region.png"), cropped)

    # 在裁剪图像上进行中点查找
    gray = cv2.cvtColor(cropped, cv2.COLOR_BGR2GRAY)
    _, binary = cv2.threshold(gray, 150, 255, cv2.THRESH_BINARY_INV)

    crop_height, crop_width = binary.shape
    y_direction = 1 if crop_y_start < crop_y_end else -1

    curve_points = []
    curve_points_real = []

    for x_crop in range(crop_width):
        # Map cropped x to original image x
        x_orig = x_crop + crop_x_start
        # Get column in cropped image
        col = binary[:, x_crop] if y_direction > 0 else binary[::-1, x_crop]
        black_indices = np.where(col == 255)[0]

        if len(black_indices) > 0:
            y_top_crop = black_indices[0]
            y_bottom_crop = black_indices[-1]
            y_center_crop = (y_top_crop + y_bottom_crop) / 2  # Use float for precision
            # Map cropped y to original image y
            y_orig = y_center_crop + crop_y_start if y_direction > 0 else crop_y_end - y_center_crop

            # Ensure y_orig is within image bounds
            y_orig = max(0, min(y_orig, height - 1))

            curve_points.append([int(x_orig), int(y_orig)])

            # 像素 → 实际坐标转换 (use float for precision)
            if x_middle is not None:
                if x_orig <= x_middle:
                    x_real = 4000.0 - (2000.0 * (x_orig - x_start) / (x_middle - x_start))
                else:
                    x_real = 2000.0 - (2000.0 * (x_orig - x_middle) / (x_end - x_middle))
            else:
                x_real = 4000.0 * (x_end - x_orig) / (x_end - x_start)

            y_real = 100.0 * (y_end - y_orig) / (y_end - y_start) if y_end != y_start else 0.0
            curve_points_real.append([round(x_real, 2), round(y_real, 2)])

            # Log the first few points for debugging
            if len(curve_points) <= 5:
                print(f"Point {len(curve_points)}: x_crop={x_crop}, y_center_crop={y_center_crop:.2f}, x_orig={x_orig}, y_orig={y_orig:.2f}, x_real={x_real:.2f}, y_real={y_real:.2f}")

    # Log range of points
    if curve_points:
        x_origs = [p[0] for p in curve_points]
        x_reals = [p[0] for p in curve_points_real]
        print(f"x_orig range: {min(x_origs)} to {max(x_origs)}")
        print(f"x_real range: {min(x_reals):.2f} to {max(x_reals):.2f}")

    # 可视化结果（在原图上）
    for point in curve_points:
        cv2.circle(img, tuple(point), 1, (0, 0, 255), -1)

    result_path = os.path.join(uuid_path, "result_curve.png")
    cv2.imwrite(result_path, img)
    np.save(os.path.join(uuid_path, "curve_points.npy"), np.array(curve_points))

    # Log output summary
    print(f"Total points detected: {len(curve_points)}")
    return jsonify({
        'points': curve_points,
        'points_real': curve_points_real,
        'result_image': 'result_curve.png',
        'point_count': len(curve_points)
    })

@app.route("/cv/find/peak", methods=["POST"])
def cv_find_peak():
    uuid = request.json.get("UUID")
    if not uuid:
        print("Missing uuid")
        return jsonify({'error': 'Missing uuid'}), 400

    uuid_path = os.path.join(app.config['UPLOAD_FOLDER'], uuid)
    if not os.path.isdir(uuid_path):
        print("UUID directory not found")
        return jsonify({'error': 'UUID directory not found'}), 404

    # 读取 result_curve.png
    img_path = os.path.join(uuid_path, "result_curve.png")
    if not os.path.exists(img_path):
        print("result_curve.png not found")
        return jsonify({'error': 'result_curve.png not found'}), 404

    # 读取图像
    pil_image = Image.open(img_path).convert("RGB")
    img = cv2.cvtColor(np.array(pil_image), cv2.COLOR_RGB2BGR)

    # 读取点文件
    points_path = os.path.join(uuid_path, "curve_points.npy")
    if not os.path.exists(points_path):
        print("curve_points.npy not found")
        return jsonify({'error': 'curve_points.npy not found'}), 404

    curve_points = np.load(points_path).tolist()  # list of [x, y]

    if len(curve_points) < 5:
        print("Too few points to detect peaks")
        return jsonify({'error': 'Too few points to detect peaks'}), 400

    # 读取标定点
    start = request.json.get("start")
    end_4000 = request.json.get("4000")
    end_0 = request.json.get("0")
    middle_2000 = request.json.get("2000")  # 可选

    if not start or not end_4000 or not end_0:
        print("Missing one of: start, 4000, 0")
        return jsonify({'error': 'Missing one of: start, 4000, 0'}), 400

    x_start = int(start["x"])
    x_end = int(end_0["x"])
    y_start = int(start["y"])
    y_end = int(end_0["y"])
    x_middle = int(middle_2000["x"]) if middle_2000 else None

    # Load previous coordinates for comparison
    coord_path = os.path.join(uuid_path, "last_coords.json")
    prev_coords = {}
    if os.path.exists(coord_path):
        with open(coord_path, 'r') as f:
            prev_coords = json.load(f)

    # Log input coordinates and compare with previous
    print(f"Current coordinates: x_start={x_start}, x_end={x_end}, y_start={y_start}, y_end={y_end}, x_middle={x_middle}")
    print(f"Previous coordinates: {prev_coords.get('coords', 'None')}")

    # Save current coordinates for next request
    coords = {'x_start': x_start, 'x_end': x_end, 'y_start': y_start, 'y_end': y_end, 'x_middle': x_middle}
    with open(coord_path, 'w') as f:
        json.dump({'coords': coords}, f)

    # 防止无效范围
    height, width = img.shape[:2]
    x_start = max(0, min(x_start, width - 1))
    x_end = max(0, min(x_end, width - 1))
    y_start = max(0, min(y_start, height - 1))
    y_end = max(0, min(y_end, height - 1))
    if x_middle is not None:
        x_middle = max(0, min(x_middle, width - 1))

    if x_end <= x_start:
        print("Invalid x calibration range")
        return jsonify({"error": "x_start must be less than x_end"}), 400
    if y_end == y_start:
        print("Invalid y calibration range")
        return jsonify({"error": "y_start and y_end are identical, cannot compute real y-values"}), 400
    if x_middle is not None and (x_middle <= x_start or x_middle >= x_end):
        print("Invalid middle_2000 x-coordinate")
        return jsonify({'error': 'Invalid middle_2000 x-coordinate'}), 400

    # 提取 y 序列
    y_values = np.array([pt[1] for pt in curve_points])
    x_values = np.array([pt[0] for pt in curve_points])

    # 找峰值
    peaks, _ = find_peaks(-y_values, distance=20, prominence=2)

    peak_list = []
    peak_list_real = []
    print(f"points of x: start: {x_start}, middle: {x_middle}, end: {x_end}")
    for idx, i in enumerate(peaks):
        x, y = curve_points[i]
        print(f"Peak {idx+1} pixel: x={x}, y={y}")
        peak_list.append([int(x), int(y)])

        # 像素 → 实际坐标转换
        if x_middle is not None:
            if x <= x_middle:
                x_real = 4000.0 - (2000.0 * (x - x_start) / (x_middle - x_start))
            else:
                x_real = 2000.0 - (2000.0 * (x - x_middle) / (x_end - x_middle))
        else:
            x_real = 4000.0 * (x_end - x) / (x_end - x_start)

        y_real = 100.0 * (y_end - y) / (y_end - y_start) if y_end != y_start else 0.0
        peak_list_real.append([round(x_real, 2), round(y_real, 2)])

        # 绘制蓝圈和编号（使用像素坐标）
        cv2.circle(img, (x, y), 6, (255, 0, 0), 2)
        cv2.putText(img, f"P{idx+1}", (x + 5, y - 5),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1)

        # Log real values for debugging
        print(f"Peak {idx+1} real: x_real={x_real:.2f}, y_real={y_real:.2f}")

    # Log range of peaks
    if peak_list:
        x_origs = [p[0] for p in peak_list]
        x_reals = [p[0] for p in peak_list_real]
        print(f"Peak x_orig range: {min(x_origs)} to {max(x_origs)}")
        print(f"Peak x_real range: {min(x_reals):.2f} to {max(x_reals):.2f}")

    # 保存图像
    result_peak_path = os.path.join(uuid_path, "result_with_peaks.png")
    cv2.imwrite(result_peak_path, img)

    return jsonify({
        "peak_points": peak_list,  # Pixel coordinates
        "peak_points_real": peak_list_real,  # Real x (0-4000), y (0-100)
        "peak_count": len(peak_list),
        "result_image": "result_with_peaks.png"
    })

@app.route("/download/<uuid>/<filename>")
def download_result(uuid, filename):
    return send_file(os.path.join(app.config['UPLOAD_FOLDER'], uuid, filename), mimetype='image/png')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5020, debug=True)