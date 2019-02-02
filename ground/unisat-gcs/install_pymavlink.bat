py -m pip uninstall -y pymavlink 
set MDEF=C:\Users\MI\PycharmProjects\CanSat_20I9\common\mavlink\message_definitions
cd C:\Users\MI\PycharmProjects\CanSat_20I9\common\mavlink\pymavlink
py setup.py bdist_wheel
cd dist
py -m pip install pymavlink-2.2.8-py3-none-any.whl