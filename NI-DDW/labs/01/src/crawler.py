import requests
from bs4 import BeautifulSoup
import time


def crawler(seed):
    frontier = [seed]
    crawled = []
    data = []
    while frontier:
        print(len(frontier), len(data))
        page = frontier.pop()
        try:
            print('Crawled:' + page)
            response = requests.get(page, headers={"user-agent": "DDW"})
            source = response.text
            soup = BeautifulSoup(source, "html5lib")
            links = soup.findAll('a', href=True)

            person = soup.select_one('div.person')
            if person:
                data.append({
                    'name': person.select_one('span.name').text
                })

            if page not in crawled:
                for link in links:
                    frontier.append(seed + link['href'])
                crawled.append(page)
            time.sleep(1)

        except Exception as e:
            print(e)
    return data


people = crawler('http://localhost:8000')
print(len(people))
print(people[:3])
