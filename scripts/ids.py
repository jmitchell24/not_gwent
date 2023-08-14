import os 
import requests
import csv 
import time 

def does_file_exist(file_path):
    if not os.path.exists(file_path):
        return False

    # Check if the file has data
    with open(file_path, 'r') as file:
        data = file.read()
        if not data:
            return False
        else:
            return True

def save_card_img(file_name, url, dry_run=True): 
    if dry_run:
        print(f"download complete: file={file_name}, url={url}")
        return True 

    if (does_file_exist(file_name)): 
        print(f"download skipped : file={file_name}")
        return True 

    #url = "https://vignette.wikia.nocookie.net/witcher/images/0/07/Tw3_gwent_card_face_Clear_Weather.png/revision/latest?cb=20151026181027"
    #file_name = "Clear_Weather.png"
    response = requests.get(url)

    if response.status_code == 200:
        with open(file_name, "wb") as file:
            file.write(response.content)
        print(f"download complete: file={file_name}, url={url}")
        return True 
    else:
        print(f"download failed  : error={response.status_code}")
        return False 


def parse_csv_file(file_path): 
    with open(file_path, newline='') as csvfile: 
        csv_reader = csv.reader(csvfile) 
        header = next(csv_reader) 

        for row in csv_reader: 
            file_name = f"{os.path.dirname(os.path.abspath(__file__))}/cards/{row[0]}.png" 
            url = row[8]

            time.sleep(2)
            if (save_card_img(file_name, url, dry_run=False) == False): 
                break


file_path = f"{os.path.dirname(os.path.abspath(__file__))}/all-cards.csv"

parse_csv_file(file_path)
