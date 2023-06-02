from collections import Counter
import pandas as pd

# Constants
CLICKS_FILE = "clicks.csv"
VISITORS_FILE = "visitors.csv"
CONVERSIONS = ['Application', 'Catalog', 'Discount', 'Howtojoin', 'Insurance', 'Whoweare']


# Transform row to conversion
def row_to_conversion(row, conversion):
    for norm in ['PageName', 'CatName', 'ExtCatName']:
        if row[norm].title() in conversion:
            return True
    return False


def custom_agg(row):
    series = {}
    for conversion in CONVERSIONS:
        series[conversion] = row[row[conversion]]['TimeOnPage'].sum()
    return pd.Series(series)


def load_df():
    # Load visitors, remove short visits and unnecessary column
    df1 = pd.read_csv('data/visitors.csv')
    df1 = df1[df1['Length_seconds'] > 0]
    df1 = df1.drop(['Length_seconds', 'Length_pagecount'], axis=1)
    df1['Hour'] = pd.cut(df1['Hour'], [0, 6, 12, 18, 24], include_lowest=True)

    # Load clicks and remove unnecessary columns
    df2 = pd.read_csv('data/clicks.csv')
    df2 = df2.drop(['LocalID', 'CatID', 'ExtCatID', 'TopicID', 'PageID'], axis=1)
    for conversion in CONVERSIONS:
        df2[conversion] = df2.apply(lambda row: row_to_conversion(row, conversion), axis=1)
    df2 = df2.drop(['CatName', 'ExtCatName', 'PageName'], axis=1)
    grouped = df2.groupby('VisitID', group_keys=False).apply(lambda x: custom_agg(x))
    for conversion in CONVERSIONS:
        grouped[conversion] = pd.cut(grouped[conversion], [0, 1, 300, 10000], include_lowest=True)

    # Merge visits
    merged = pd.merge(df1, grouped, on='VisitID')
    merged = merged.drop('VisitID', axis=1)
    return merged


# Convert data frame to data set for Apriori algorithm
def convert_to_dataset(dataframe):
    dataset = []
    # combine column name and value
    for index, row in dataframe.iterrows():
        row = [f"{col}={row[col]}" for col in list(dataframe)]
        dataset.append(row)
    return dataset


# Find most frequent items in given visits set
def frequent_items(visits, min_support=0.1):
    counter = Counter()
    for trans in visits:
        counter.update(frozenset([t]) for t in trans)
    return set(item for item in counter if counter[item] / len(visits) >= min_support), counter


# Generate subsets of size `list_len` from `lst`
def generate_candidates(lst, list_len):
    candidates = set()
    for a in lst:
        for b in lst:
            union = a | b
            conv = 0
            for conversion in CONVERSIONS:
                for item in union:
                    # Do not allow rule with conversion=(-0.001, 1.0]
                    if "=(-0.001, 1.0]" in item:
                        conv = 666
                    # Do not allow {conversion} -> conversion rule
                    if conversion in item:
                        conv += 1
            if len(union) == list_len and a != b and conv <= 1:
                candidates.add(union)
    return candidates


# Filter given subset to only sets with support of `min_support`
def filter_candidates(visits, lists, min_support=0.1):
    counter = Counter()
    for visit in visits:
        subsets = [lst for lst in lists if lst.issubset(visit)]
        counter.update(subsets)
    return set(item for item in counter if counter[item] / len(visits) >= min_support), counter


# Apriori algorithm to find items with the biggest support
def apriori_algorithm(visits, min_support=0.1, max_len=4):
    candidates, counter = frequent_items(visits, min_support)  # start with items (size 1)
    result = list(candidates)
    result_counter = counter
    k = 2
    while candidates:
        # generate candidates with support
        candidates = generate_candidates(candidates, k)
        candidates, counter = filter_candidates(visits, candidates, min_support)
        # add to result
        result += candidates
        result_counter += counter
        k += 1
        # stop if set length would be too long
        if k > max_len:
            break
    result_counter = {item: (result_counter[item] / len(visits)) for item in result_counter}
    return result, result_counter


# Generate association rules from frequent item sets
def generate_rules(freq_items, supp_items, min_confidence=0.5):
    result = []
    for item in freq_items:
        # Skip {} -> {item} rules
        if len(item) == 1:
            continue

        # Find all any set -> {item} rules
        for consequent in item:
            antecedent = set(item).difference([consequent])
            support = supp_items[item]
            confidence = support / supp_items[frozenset(antecedent)]
            # not enough confidence
            if confidence < min_confidence:
                break

            # append the rule
            result.append({
                "antecedent": antecedent,
                "consequent": consequent,
                "support": support,
                "confidence": confidence,
                "len": len(item)
            })
    return pd.DataFrame(result)


# Run program
def analyze_data():
    df = load_df()
    df.to_csv('../results/merged.csv')
    print(df)
    ds = convert_to_dataset(df)

    frequent_sets, supports = apriori_algorithm(ds, 0.02)
    for frequent_set in sorted(frequent_sets, key=lambda x: supports[x], reverse=True)[:10]:
        print(list(frequent_set), supports[frequent_set], sep=', ')

    rules_df = generate_rules(frequent_sets, supports, min_confidence=0.66)
    rules_df = rules_df.sort_values(["support", "confidence"], ascending=[False, False])
    rules_df.to_csv('../results/associations.csv')
    print(len(frequent_sets), len(rules_df))


# Main
if __name__ == '__main__':
    analyze_data()
