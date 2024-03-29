# High level functions for debug variable manipulation

__copyright__ = "Copyright 2021 RespiraWorks"

__license__ = """

    Copyright 2021 RespiraWorks

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

"""

import numpy as np

SUPPORTED_FUNCTIONS = {
    "lin(low, high)": "evenly spaced numbers over low and high",
    "eigen()": "evenly spaced out numbers from 0 to 1",
}


def lin(low: float, high: float):
    return np.linspace(low, high, 11)


def eigen():
    return lin(0, 1)


def scoped_eval(string_to_eval):
    return eval(string_to_eval)
