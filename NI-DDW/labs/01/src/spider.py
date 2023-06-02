import scrapy


class BlogSpider(scrapy.Spider):
    name = 'ddw-spider'
    start_urls = ['http://localhost:8000']
    custom_settings = {
        'USER_AGENT': 'DDW',
        'DOWNLOAD_DELAY': 1,
        'RANDOMIZE_DOWNLOAD_DELAY': False
    }

    def parse(self, response):
        for person in response.css('div.person'):
            yield {'name': person.css('span.name ::text').extract_first()}

        next_page = response.css('a ::attr(href)').extract_first()
        if next_page:
            yield scrapy.Request(response.urljoin(next_page), callback=self.parse)
