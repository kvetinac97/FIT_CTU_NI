import unicodedata
import scrapy


class BlogSpider(scrapy.Spider):
    # initial setup
    name = 'ddw-spider'
    start_urls = ['https://www.novinky.cz/sitemaps/sitemap_sections.xml']
    allowed_domains = ['novinky.cz']  # allow crawling only Novinky site

    # advanced setup
    custom_settings = {
        'USER_AGENT': 'FIT-CVUT-DDW',
        # in robots.txt, the delay is not defined,
        # so I decided to use around 1 second
        'DOWNLOAD_DELAY': 1,
        'RANDOMIZE_DOWNLOAD_DELAY': True,
        # important: set how many pages should be extracted here
        'DEPTH_LIMIT': 3,
        # export settings
        'FEED_FORMAT': 'json',
        'FEED_EXPORT_ENCODING': 'utf-8'
    }

    # parse sections from sitemap
    def parse(self, response):
        sitemapNs = [['n', 'http://www.sitemaps.org/schemas/sitemap/0.9']]
        for section_url in response.xpath('/n:urlset/n:url/n:loc/text()', namespaces=sitemapNs):
            yield scrapy.Request(response.urljoin(section_url.get()), callback=self.parse_section)

    def parse_section(self, response):
        for article in response.css('article'):
            article_type = article.css('::attr(data-dot)').extract_first()
            match article_type:
                # parse articles
                case 'document-item':
                    yield {
                        'title': self.normalize(article.css('div.q_gL a.c_al ::text').extract_first()),
                        'url': article.css('div.q_gL a.c_al ::attr(href)').extract_first(),
                        'image_url': self.fix_image_url(article.css('img.c_n ::attr(src)').extract_first()),
                        'time': article.css('div.q_gM span.atm-date ::text').extract_first(),
                        'shorttext': self.normalize(article.css('p.q_gP ::text').extract_first())
                    }
                # skip unwanted items
                case 'ogm-top-stories-item' | 'ogm-article-content' | 'advert-item' | 'feed-item':
                    continue
                case _:
                    print(f"Unknown type {article_type} {article.get()}")

        # Look for "Next page" button
        next_page = response.css('a.c_al.c_aR')
        for anchor in next_page:
            if "další" in anchor.css('::text').extract_first():
                next_url = anchor.css('::attr(href)').extract_first()
                yield scrapy.Request(response.urljoin(next_url), callback=self.parse_section)

    # Helper method to normalize invalid characters
    @staticmethod
    def normalize(text):
        return unicodedata.normalize("NFKD", text or '')

    # Helper method to fix
    @staticmethod
    def fix_image_url(url):
        if url:
            return 'https:' + url
        else:
            return 'None'
