#!/usr/bin/env python3

import pandas
import json

url = "https://docs.google.com/spreadsheets/d/e/2PACX-1vRduOfterWmAy_xrc356rRhjz4QDLgOScgG1VPx2-KNeH8zYEe29SCw_DKOJG-5hqSO6BXmG1BumUul/pub?gid=0&single=true&output=tsv"
data = json.loads(pandas.read_csv(url, sep="\t", header=0).to_json(orient="records"))
print(json.dumps(data, indent=4))
