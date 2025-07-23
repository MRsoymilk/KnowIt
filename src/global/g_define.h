#ifndef G_DEFINE_H
#define G_DEFINE_H

#include <QString>

const QString DELIMITER = ";";

const QString BASIC_INFORMATION = "Basic Information";
const QString SPECTRAL_DATA = "Spectral Data";
const QString SPECTRAL_ACQUISITION_CONDITIONS = "Spectral Acquisition Conditions";
const QString SAMPLE_INFOMATION = "Sample Information";
const QString ANNOTATIONS_AND_TAGS = "Annotations And Tags";
const QString ORIGINATE = "Originate";

// Basic Information
const QString ID = "ID";
const QString COMPOUND_NAME_EN = "Compound Name(en)";
const QString COMPOUND_NAME_ZH = "Compound Name(zh)";
const QString MOLECULAR_FORMULA = "Molecular Formula";
const QString MOLECULAR_WEIGHT = "Molecular Weight";
const QString CAS_NUMBER = "CAS Number";
const QString COMPOUND_TYPE = "Compound Type";
const QString STRUCTURE_PICTURE = "Structure Picture";
const QString STATE = "State";
// Spectral Data
const QString PEAK_MAIN = "Peak(Main)";
const QString PEAK_ASSISTANCE = "Peak(Assistance)";
const QString SPECTRAL_PICTURE = "Spectral Picture";
// Spectral Acquisition Conditions
const QString INSTRUMENT_MODEL = "Instrument Model";
const QString LASER_WAVELENGTH = "Laser Wavelength";
const QString LASER_POWER = "Laser Power";
const QString CALCULUS_TIMES = "Calculus Times";
const QString MEASURING_ENVIRONMENT = "Measuring Environment";
const QString MEASURING_TEMPERATURE = "Measuring Temperature";
// Sample Information
const QString PRODUCED_FROM = "Produced From";
const QString STORAGE = "Storage";
const QString DERIVATIVE_STATE = "Derivative State";
// Annotations and Tags
const QString CATEGORY = "Category";
const QString APPLICATION_AREA = "Application Area";
const QString DATA_VERSION = "Data Version";
const QString LAST_MODIFIED = "Last Modified";
// Originate
const QString RESOURCE = "Resource";

// Server
const QString GROUP_SERVER = "SERVER";
const QString URL_SERVER = "url_server";
const QString PATH_PING = "path_ping";
const QString PATH_UPLOAD_IMG = "path_upload_img";
const QString PATH_LOAD_IMG = "path_load_img";
const QString PATH_DATASET_CHECK = "path_dataset_check";
const QString PATH_DATASET_SET = "path_dataset_set";
const QString PATH_DATASET_GET = "path_dataset_get";
const QString PATH_DATASET_DEL = "path_dataset_del";
const QString PATH_HTML_ANALYSIS = "path_html_analysis";
const QString KEY_URL_RAMAN_SPECTRAL_VIEW = "url_raman_spectral_view";
const QString KEY_URL_COMPOUND_VIEW = "url_compound_view";
const QString PATH_CV_FIND_CROP = "path_cv_find_crop";
const QString PATH_CV_FIND_PEAK = "path_cv_find_peak";
const QString PATH_CV_FIND_CURVE = "path_cv_find_curve";
const QString GROUP_CALIBRATION = "CALIBRATION";
const QString CALIBRATION_START = "start";
const QString CALIBRATION_4000 = "4000";
const QString CALIBRATION_2000 = "2000";
const QString CALIBRATION_0 = "0";

const QString DFT_URL_SERVER = "http://192.168.123.233:5020";
const QString DFT_PATH_PING = "/ping";
const QString DFT_PATH_UPLOAD_IMG = "/upload/img";
const QString DFT_PATH_LOAD_IMG = "/load/img";
const QString DFT_PATH_DATASET_CHECK = "/dataset/check";
const QString DFT_PATH_DATASET_SET = "/dataset/set";
const QString DFT_PATH_DATASET_GET = "/dataset/get";
const QString DFT_PATH_DATASET_DEL = "/dataset/del";
const QString DFT_PATH_HTML_ANALYSIS = "/html/analysis";
const QString DFT_PATH_CV_FIND_CROP = "/cv/find/crop";
const QString DFT_PATH_CV_FIND_PEAK = "/cv/find/peak";
const QString DFT_PATH_CV_FIND_CURVE = "/cv/find/curve";
const QString DFT_CALIBRATION_START = "(0, 0)";
const QString DFT_CALIBRATION_4000 = "(0, 0)";
const QString DFT_CALIBRATION_2000 = "(0, 0)";
const QString DFT_CALIBRATION_0 = "(0, 0)";

const QString CFG_GROUP_PROGRAM = "Program";
const QString CFG_PROGRAM_THEME = "theme";
const QString CFG_PROGRAM_LANGUAGE = "language";
const QString CFG_GROUP_PROPERTY = "Property";
const QString CFG_PROPERTY_STRUCTURE_CROP = "structure_crop";
const QString DFT_CFG_PROPERTY_STRUCTURE_CROP = "100";
const QString CFG_GROUP_CHEMICAL_CATEGORY = "ChemicalCategory";
const QString CHEMICAL_CATEGORY_MAJOR_MINOR = "ChemicalCategory_MajorMinor";
// function
#include "mysetting.h"
#define SETTING_CONFIG_GET(group, key, ...) MY_SETTING.getValue(MySetting::SETTING::CONFIG, group, key, ##__VA_ARGS__)
#define SETTING_CONFIG_SET(group, key, value) MY_SETTING.setValue(MySetting::SETTING::CONFIG, group, key, value)
#define SETTING_CONFIG_GROUPS() MY_SETTING.getGroups(MySetting::SETTING::CONFIG)
#define SETTING_CONFIG_GROUP_KEYS(group) MY_SETTING.getGroupKeys(MySetting::SETTING::CONFIG, group)
#define SETTING_CONFIG_SYNC() MY_SETTING.sync(MySetting::SETTING::CONFIG)

#include "mylog.h"
#define LOG_TRACE(...) MY_LOG.getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) MY_LOG.getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) MY_LOG.getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) MY_LOG.getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) MY_LOG.getLogger()->critical(__VA_ARGS__)

#endif  // G_DEFINE_H
