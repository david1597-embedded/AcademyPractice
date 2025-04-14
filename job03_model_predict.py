from PIL import Image
import numpy as np
from keras.models import load_model



model=load_model('./models/cat_dog_model_0.841.h5')
categories=['cat','dog']
img =Image.open('C:/Users/USER/Downloads/dog_test01.jpeg')
img=img.resize((64,64))
img=np.array(img)
img=img/255
img=img.reshape(1,64,64,3)
pred=model.predict(img)

print(pred)
print(categories[int(np.around(pred))])