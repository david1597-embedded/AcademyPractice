import numpy as np
import matplotlib.pyplot as plt
from keras.models import Sequential
from keras.layers import Dense, Flatten, Conv2D, MaxPooling2D
from keras.callbacks import EarlyStopping
import tensorflow as tf
import os

from keras.src.metrics.accuracy_metrics import binary_accuracy
from tensorflow.python.eager.profiler_client import monitor

from catanddog.job01_preprocessing import Y_train

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

# 데이터 로드
x_train = np.load('binary_data/cat_dog_x_train.npy')
x_test = np.load('binary_data/cat_dog_x_test.npy')
y_train = np.load('binary_data/cat_dog_y_train.npy')
y_test = np.load('binary_data/cat_dog_y_test.npy')

print(x_train.shape)
print(x_test.shape)
print(y_train.shape)
print(y_test.shape)

# 모델 정의
model = Sequential()

model.add(Conv2D(32, kernel_size=(3, 3), padding="same", activation='relu', input_shape=(64, 64, 3)))
model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))

model.add(Conv2D(32, kernel_size=(3, 3), padding="same", activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))

model.add(Conv2D(32, kernel_size=(3, 3), padding="same", activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))

model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(128, activation='relu'))  # 'activate' -> 'activation'
model.add(Dense(1, activation='sigmoid')) # 'activate' -> 'activation')
#model.add(Dense(1, activation=''))
# 모델 요약 출력
model.summary()

# 모델 컴파일 및 학습 (추가 예시)
model.compile(loss='binary_crossentropy', optimizer='adam',metrics=['binary_accuracy'])
earlystopping=EarlyStopping(monitor='val_binary_accuracy', patience=7)
fit_hist=model.fit(x_train, y_train, batch_size=32, epochs=100,
                   validation_data=(x_test, y_test),callbacks=[earlystopping])
score=model.evaluate(x_test, y_test)
print('Evaluation loss : ', score[0])
print('Evaluation accuracy : ', score[1])
model.save('./models/cat_dog_model_{}.h5'.format(np.around(score[1],3)))
plt.plot(fit_hist.history['binary_accuracy'])
plt.plot(fit_hist.history['val_binary_accuracy'])
plt.legend(['train', 'test'], loc='upper left')
plt.show()
plt.plot(fit_hist.history['loss'])
plt.plot(fit_hist.history['val_loss'])
plt.legend(['train', 'test'], loc='upper left')
plt.show()
