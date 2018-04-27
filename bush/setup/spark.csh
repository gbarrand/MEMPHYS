
setenv SPARK_HOME /usr/local/spark/2.3.0/spark-2.3.0-bin-hadoop2.7
setenv PYSPARK_SUBMIT_ARGS "--master local[2]"

# Make pyspark available anywhere
setenv PATH "${SPARK_HOME}/bin:$PATH"


