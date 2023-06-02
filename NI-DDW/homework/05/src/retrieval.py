# import
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.metrics.pairwise import euclidean_distances
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import CountVectorizer
import locale
import numpy as np
import nltk
from gensim.models import Word2Vec
from numpy.linalg import norm

# TODO: download cranfield collection and extract documents, queries, results
# How many TOP relevant items we should compare
TOP_RELEVANT = 15


# Prepare corpus of documents
def prepare_corpus_d():
    corpus = []
    for d in range(1400):
        corpus_file = open("./cranfield/d/" + str(d + 1) + ".txt")
        corpus.append(corpus_file.read())
    return corpus


# Add query to corpus of documents and return relevant docs
def prepare_corpus_qr(corpus, qid):
    query_file = open("./cranfield/q/" + str(qid) + ".txt")
    corpus.append(query_file.read())

    relevant_file = open("./cranfield/r/" + str(qid) + ".txt")
    relevant_docs = [int(x) for x in relevant_file.read().splitlines()]
    return relevant_docs


# Represent documents and query in Vector Space Model
def transform_binary(corpus):
    binary_vectorizer = CountVectorizer(binary=True)
    binary_matrix = binary_vectorizer.fit_transform(corpus)
    return binary_matrix[len(corpus) - 1], binary_matrix[0:(len(corpus) - 1)]


def transform_term_freq(corpus):
    freq_vectorizer = CountVectorizer()
    freq_matrix = freq_vectorizer.fit_transform(corpus)
    return freq_matrix[len(corpus) - 1], freq_matrix[0:(len(corpus) - 1)]


def transform_tf_idf(corpus):
    tfidf_vectorizer = TfidfVectorizer()
    tfidf_matrix = tfidf_vectorizer.fit_transform(corpus)
    return tfidf_matrix[len(corpus) - 1], tfidf_matrix[0:(len(corpus) - 1)]


def transform_word2vec(corpus):
    model, words = word2vec_model(corpus)
    matrix = np.zeros((len(corpus), len(words)))
    for i, doc in enumerate(corpus):
        for token in enumerate(nltk.word_tokenize(doc)):
            if model.wv.__contains__(token):
                j = words.index(token)
                matrix[i][j] = norm(model.wv[token])  # put norm of vector if present
    return [matrix[len(corpus) - 1]], matrix[0:(len(corpus) - 1)]


# Help function for building Word2Vec model
def word2vec_model(corpus):
    model = Word2Vec(window=5, min_count=2, workers=4, sg=0)
    words = []
    # add word tokens
    for i, doc in enumerate(corpus):
        for token in nltk.word_tokenize(doc):
            words.append(token)
    model.build_vocab(words, progress_per=1000)
    model.train(words, total_examples=model.corpus_count, epochs=model.epochs)
    return model, words


# Return TOP len(relevant) objects using given matrix and similarity functions
def top_relevant(q, docs, sim_fn):
    topSimilar = np.array(sim_fn(q, docs)[0])
    return topSimilar.argsort()[-TOP_RELEVANT:][::-1] + 1


# Calculate precision, recall and F-score
def calculate_prf(result, reference):
    if len(result) == 0 or len(reference) == 0:
        return 0, 0, 0

    good = 0
    # precision
    for doc in result:
        if doc in reference:
            good += 1
    precision = good / len(result)

    # recall
    good = 0
    for doc in reference:
        if doc in result:
            good += 1
    recall = good / len(reference)

    # f-score
    if precision == 0 or recall == 0:
        return precision, recall, 0

    fs = 2 * ((precision * recall) / (precision + recall + 1))
    return precision, recall, fs


if __name__ == '__main__':
    locale.setlocale(locale.LC_ALL, 'cs_CZ')  # use "," as decimal separator
    nltk.download('punkt')  # needed for tokenization
    print("query_id;"
          "bin_cos_p;bin_cos_r;bin_cos_f;bin_euc_p;bin_euc_r;bin_euc_f;"
          "ter_cos_p;ter_cos_r;ter_cos_f;ter_euc_p;ter_euc_r;ter_euc_f;"
          "tid_cos_p;tid_cos_r;tid_cos_f;tid_euc_p;tid_euc_r;tid_euc_f;"
          "w2v_cos_p;w2v_cos_r;w2v_cos_f;w2v_euc_p;w2v_euc_r;w2v_euc_f")
    corpus_dq = prepare_corpus_d()
    for query_id in range(225):
        relevant = prepare_corpus_qr(corpus_dq, query_id + 1)
        print(query_id + 1, end=";")
        for matrix_fn in [transform_binary, transform_term_freq, transform_tf_idf, transform_word2vec]:
            query, documents = matrix_fn(corpus_dq)
            for similarity_fn in [cosine_similarity, euclidean_distances]:
                topRelevant = top_relevant(query, documents, similarity_fn)
                p, r, f = calculate_prf(topRelevant, relevant)
                print(locale.format_string("%f;%f;%f", (p, r, f)), end="")
                if similarity_fn != euclidean_distances:
                    print(";", end="")
            if matrix_fn != transform_word2vec:
                print(";", end="")
        print()  # newline
