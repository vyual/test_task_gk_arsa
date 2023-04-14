import requests
import json

# Тестирование метода /api/v1/get_sum

# Подставьте свой адрес сервера
url1 = "http://localhost:8080/api/v1/get_sum?a=5&b=7"
# Подставьте свои значения a и b
url2 = "http://localhost:8080/api/v1/managers?name=Vasily"
# Отправляем GET запрос
response1 = requests.post(url1)
response2 = requests.post(url2)
print(response1)
print(response2)

# Десериализуем полученный JSON
result1 = json.loads(response1.text)
print(result1)
result2 = json.loads(response2.text)
print(result2)

