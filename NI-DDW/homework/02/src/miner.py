import collections
import nltk
from nltk import RegexpParser
from transformers import AutoTokenizer, AutoModelForTokenClassification
from transformers import pipeline
import wikipedia


# Open file
def read_text(file):
    with open(file) as script:
        return "".join(script.readlines())


# POS tagging
def pos_tagging(text):
    sentences = nltk.sent_tokenize(text)
    tokens = [nltk.word_tokenize(sent) for sent in sentences]
    tagged = [nltk.pos_tag(sent) for sent in tokens]  # run part-of-speech on each sentence
    return tagged


# Extract entities from tree structure
def extract_entities(ne_chunked, type):
    data = []
    for entity in ne_chunked:
        if isinstance(entity, nltk.tree.Tree):
            contents = " ".join([word for word, tag in entity.leaves()])
            ent = entity.label()
            if not type:
                data.append(contents)
            else:
                data.append(f"{contents} ({ent})")
        else:
            continue
    return data


# NER with entity classification
def ner_ec(text, type):
    tokens = nltk.word_tokenize(text)
    tagged = nltk.pos_tag(tokens)

    ne_chunked = nltk.ne_chunk(tagged, binary=False)  # get categories
    return extract_entities(ne_chunked, type)


# NER with custom pattern
def ner_cp(text):
    # e.g. every match of: adjective (optional) and proper noun (singular/plural) is matched as the entity
    tokens = nltk.word_tokenize(text)
    tagged = nltk.pos_tag(tokens)

    ne = []
    entity = []
    for tagged_entry in tagged:
        # match a sequence of adjectives or pronouns finished by proper noun with uppercase letter
        if tagged_entry[1].startswith("JJ") or tagged_entry[1].startswith("DT"):
            if tagged_entry[1].startswith("JJ"):
                entity.append(tagged_entry)  # append adjective to list
            # "the" is not appended
        else:
            # found upper noun, add as named entity
            if tagged_entry[1].startswith("NN") and tagged_entry[0][0].isupper():
                entity.append(tagged_entry)
                ne.append(" ".join(e[0] for e in entity))
            entity = []
    return ne


# NER recognition using existing language model BERT
def ner_bert(text):
    model = AutoModelForTokenClassification.from_pretrained("dslim/bert-base-NER")
    tokenizer = AutoTokenizer.from_pretrained("dslim/bert-base-NER")

    nlp = pipeline("ner", model=model, tokenizer=tokenizer, aggregation_strategy="simple")
    return nlp(text)


# Custom entity classification
def classify_wikipedia(entity):
    # Try to get page exactly
    try:
        page = wikipedia.summary(entity, sentences=1, auto_suggest=False)
    # If there is disambiguaty, use the first alternative
    except wikipedia.DisambiguationError as e:
        page = wikipedia.summary(e.options[0], sentences=1, auto_suggest=False)
    except wikipedia.PageError:
        return "Thing"

    text_pos = nltk.pos_tag(nltk.word_tokenize(page))
    regex = RegexpParser("NP: <VBZ> <DT>? {<JJ>* <NN>+}")
    result = regex.parse(text_pos)

    for word in result:
        if isinstance(word, nltk.tree.Tree):
            text = " ".join([word for word, tag in word.leaves()])
            return text
    return "Thing"


# Testing
t = read_text('transcript.txt')
print(pos_tagging(t)[:10])
print("")

result_ec = sorted(list(dict(collections.Counter(ner_ec(t, type=True))).items()), key=lambda e: e[1], reverse=True)[:10]
result_cp = sorted(list(dict(collections.Counter(ner_cp(t))).items()), key=lambda e: e[1], reverse=True)[:10]

print(result_ec)
print(result_cp)
print(ner_bert(t))

result_ec_unnamed = sorted(list(dict(collections.Counter(ner_ec(t, type=False))).items()),
                           key=lambda e: e[1], reverse=True)[:10]
entities = set(dict.fromkeys(result_ec_unnamed + result_cp))
for ent in entities:
    print({ent[0]: classify_wikipedia(ent[0].lower())})

