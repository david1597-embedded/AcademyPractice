import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5 import uic
from PIL import Image
import numpy as np
from keras.models import load_model
import cv2

print(cv2.__version__)
form_class = uic.loadUiType("./cat_and_dog.ui")[0]

class ExampleApp(QWidget, form_class):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.path=('./images/train/cat.2.jpg','')
        self.btn_open.clicked.connect(self.btn_clicked_slot)
        self.model = load_model('./models/cat_dog_model_0.841.h5')


    def btn_clicked_slot(self):
        old_path=self.path
        self.path = QFileDialog.getOpenFileName(
            self, "Open File", "./images",
            'Image Files (*.jpg *.jpeg *.png);;All Files (*)')
        print(self.path)
        if self.path[0] == '':
            self.path = old_path

        try:
            pixmap = QPixmap(self.path[0])
            self.lbl_image.setPixmap(pixmap)
            img = Image.open(self.path[0])
            img = img.convert('RGB')
            img = img.resize((64, 64))
            img = np.array(img)
            img = img / 255
            img = img.reshape(1, 64, 64, 3)
            pred = self.model.predict(img)
            print(pred)
            if pred[0][0] > 0.5:
                self.lbl_result.setText('DOG')
            else :
                self.lbl_result.setText('CAT')
        except:
            print('error')



if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_window = ExampleApp()
    main_window.show()
    sys.exit(app.exec_())
