#define BOOST_TEST_MODULE BigIntegerTestMultiply
#include <boost/test/unit_test.hpp>

#include "../src/biginteger/biginteger.hpp"

//Ground truth for calculation is https://www.calculator.net/big-number-calculator.html

// check group axioms https://en.wikipedia.org/wiki/Group_(mathematics)
// identity element
BOOST_AUTO_TEST_SUITE(IDENTITY_ELEMENT)
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ONE_LENGTH1_184_LENGTH2_1_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("1");
    BOOST_CHECK_EQUAL(b.getNumber(), "7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ONE_LENGTH1_1_LENGTH2_184_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("1");
    b.multiply("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ONE_LENGTH1_184_LENGTH2_1_SIGN1_MINUS_SIGN2_PLUS)
{
    BigInteger b("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("1");
    BOOST_CHECK_EQUAL(b.getNumber(), "-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ONE_LENGTH1_1_LENGTH2_184_SIGN1_PLUS_SIGN2_MINUS)
{
    BigInteger b("1");
    b.multiply("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
}
BOOST_AUTO_TEST_SUITE_END()

// inverse element - techincally doesnt exist for integerdivision
// so instead i will check multiplication with zero here, again no -0 here since constructor tests checked for it to be equal to 0
BOOST_AUTO_TEST_SUITE(INVERSE_ELEMENT)
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ZERO_LENGTH1_184_LENGTH2_1_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("0");
    BOOST_CHECK_EQUAL(b.getNumber(), "0");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ZERO_LENGTH1_1_LENGTH2_184_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("0");
    b.multiply("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "0");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ZERO_LENGTH1_184_LENGTH2_1_SIGN1_MINUS_SIGN2_PLUS)
{
    BigInteger b("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("0");
    BOOST_CHECK_EQUAL(b.getNumber(), "0");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_ZERO_LENGTH1_1_LENGTH2_184_SIGN1_PLUS_SIGN2_MINUS)
{
    BigInteger b("0");
    b.multiply("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "0");
}
BOOST_AUTO_TEST_SUITE_END()

// equal number
BOOST_AUTO_TEST_SUITE(EQUAL_NUMBER)
BOOST_AUTO_TEST_CASE(MULTIPLICATION_EQUAL_LENGTH1_184_LENGTH2_184_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "63064819009189351381531322863622429225416678227323980466192870945341251245872737222737065026337639037339894551523993376756300746426825878176791598739025037306183146697559099688401418713875595206040901291779332501628265467442058595218584170449327918540109413318691738819602695423230689728759982782597196144989775570304299850656702955503689956594763654795196770577048900");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_EQUAL_LENGTH1_184_LENGTH2_184_SIGN1_MINUS_SIGN2_PLUS)
{
    BigInteger b("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-63064819009189351381531322863622429225416678227323980466192870945341251245872737222737065026337639037339894551523993376756300746426825878176791598739025037306183146697559099688401418713875595206040901291779332501628265467442058595218584170449327918540109413318691738819602695423230689728759982782597196144989775570304299850656702955503689956594763654795196770577048900");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_EQUAL_LENGTH1_184_LENGTH2_184_SIGN1_PLUS_SIGN2_MINUS)
{
    BigInteger b("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-63064819009189351381531322863622429225416678227323980466192870945341251245872737222737065026337639037339894551523993376756300746426825878176791598739025037306183146697559099688401418713875595206040901291779332501628265467442058595218584170449327918540109413318691738819602695423230689728759982782597196144989775570304299850656702955503689956594763654795196770577048900");
}
BOOST_AUTO_TEST_CASE(MULTIPLICATION_EQUAL_LENGTH1_184_LENGTH2_184_SIGN1_MINUS_SIGN2_MINUS)
{
    BigInteger b("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "63064819009189351381531322863622429225416678227323980466192870945341251245872737222737065026337639037339894551523993376756300746426825878176791598739025037306183146697559099688401418713875595206040901291779332501628265467442058595218584170449327918540109413318691738819602695423230689728759982782597196144989775570304299850656702955503689956594763654795196770577048900");
}
BOOST_AUTO_TEST_SUITE_END()

// inequal length
BOOST_AUTO_TEST_SUITE(INEQUAL_LENGTH)
BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_184_LENGTH2_251_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("10472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357457230857106002180626509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "83162300745558161330188232956006555985297356671813668658541194833188480108767282191396806374670357418674173971690263173467716278151766329223082208614813823728987243659277172060372080062510871263920962423824485546211833592241044332458885117842939392151744623029561764067066711903840555347479153884293614650552856215780487486041476066707413005194055513263769059431306179601908165157846394599887984917660421563777478282239194755577048900");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_251_LENGTH2_184_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("10472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357457230857106002180626509330");
    b.multiply("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "83162300745558161330188232956006555985297356671813668658541194833188480108767282191396806374670357418674173971690263173467716278151766329223082208614813823728987243659277172060372080062510871263920962423824485546211833592241044332458885117842939392151744623029561764067066711903840555347479153884293614650552856215780487486041476066707413005194055513263769059431306179601908165157846394599887984917660421563777478282239194755577048900");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_184_LENGTH2_251_SIGN1_MINUS_SIGN2_PLUS)
{
    BigInteger b("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("10472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357457230857106002180626509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-83162300745558161330188232956006555985297356671813668658541194833188480108767282191396806374670357418674173971690263173467716278151766329223082208614813823728987243659277172060372080062510871263920962423824485546211833592241044332458885117842939392151744623029561764067066711903840555347479153884293614650552856215780487486041476066707413005194055513263769059431306179601908165157846394599887984917660421563777478282239194755577048900");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_184_LENGTH2_251_SIGN1_PLUS_SIGN2_MINUS)
{
    BigInteger b("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    b.multiply("-10472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357457230857106002180626509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-83162300745558161330188232956006555985297356671813668658541194833188480108767282191396806374670357418674173971690263173467716278151766329223082208614813823728987243659277172060372080062510871263920962423824485546211833592241044332458885117842939392151744623029561764067066711903840555347479153884293614650552856215780487486041476066707413005194055513263769059431306179601908165157846394599887984917660421563777478282239194755577048900");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_251_LENGTH2_184_SIGN1_MINUS_SIGN2_PLUS)
{
    BigInteger b("-10472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357457230857106002180626509330");
    b.multiply("7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-83162300745558161330188232956006555985297356671813668658541194833188480108767282191396806374670357418674173971690263173467716278151766329223082208614813823728987243659277172060372080062510871263920962423824485546211833592241044332458885117842939392151744623029561764067066711903840555347479153884293614650552856215780487486041476066707413005194055513263769059431306179601908165157846394599887984917660421563777478282239194755577048900");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_251_LENGTH2_184_SIGN1_PLUS_SIGN2_MINUS)
{
    BigInteger b("10472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357457230857106002180626509330");
    b.multiply("-7941336097231331222328305438625646002846406186146402399842122390970234239963370898300943198440768677925436553855703282917116959906917075742261367262117888460342170898243453976126509330");
    BOOST_CHECK_EQUAL(b.getNumber(), "-83162300745558161330188232956006555985297356671813668658541194833188480108767282191396806374670357418674173971690263173467716278151766329223082208614813823728987243659277172060372080062510871263920962423824485546211833592241044332458885117842939392151744623029561764067066711903840555347479153884293614650552856215780487486041476066707413005194055513263769059431306179601908165157846394599887984917660421563777478282239194755577048900");
}
BOOST_AUTO_TEST_SUITE_END()

// equal length
BOOST_AUTO_TEST_SUITE(EQUAL_LENGTH)
BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_300_LENGTH2_300_SIGN1_PLUS_SIGN2_PLUS)
{
    BigInteger b("723085710600218062650933023587923476589623894235324510472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357453244654778967912376582");
    b.multiply("865974983284573850973248597342896572389732589237459235769327452834737892357357326823409812079108497283450964987345678562913402135007658050090132094237476126465235676757786798009856890546097809408787512737541726847621635416573127646523675742657935769784537806784370579324523453479126734810209109091901");
    BOOST_CHECK_EQUAL(b.getNumber(), "626174136150338041849666678564161717337444215341554788470550319933736924219468731873674467654024358677304657705415675769073489852097177950401568476377333970545290590106007377782540002258424527629550039988395207817393754072116158892205344988532909731556149781263035042104847401378563610300473891708122024090216953246843365056183746195106883235661782512398628023983212765032808921990306665465403723029153970364656627662363482763741389227339999225082549958423571348595687840437395971355976733543903458496226172620315500993730954093139516157090481698551239561121825961463820441226732988250504800758262382");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_300_LENGTH2_300_SIGN1_PLUS_SIGN2_PLUS_SWAPPED)
{
    BigInteger b("865974983284573850973248597342896572389732589237459235769327452834737892357357326823409812079108497283450964987345678562913402135007658050090132094237476126465235676757786798009856890546097809408787512737541726847621635416573127646523675742657935769784537806784370579324523453479126734810209109091901");
    b.multiply("723085710600218062650933023587923476589623894235324510472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357453244654778967912376582");
    BOOST_CHECK_EQUAL(b.getNumber(), "626174136150338041849666678564161717337444215341554788470550319933736924219468731873674467654024358677304657705415675769073489852097177950401568476377333970545290590106007377782540002258424527629550039988395207817393754072116158892205344988532909731556149781263035042104847401378563610300473891708122024090216953246843365056183746195106883235661782512398628023983212765032808921990306665465403723029153970364656627662363482763741389227339999225082549958423571348595687840437395971355976733543903458496226172620315500993730954093139516157090481698551239561121825961463820441226732988250504800758262382");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_300_LENGTH2_300_SIGN1_MINUS_SIGN2_PLUS)
{
    BigInteger b("-723085710600218062650933023587923476589623894235324510472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357453244654778967912376582");
    b.multiply("865974983284573850973248597342896572389732589237459235769327452834737892357357326823409812079108497283450964987345678562913402135007658050090132094237476126465235676757786798009856890546097809408787512737541726847621635416573127646523675742657935769784537806784370579324523453479126734810209109091901");
    BOOST_CHECK_EQUAL(b.getNumber(), "-626174136150338041849666678564161717337444215341554788470550319933736924219468731873674467654024358677304657705415675769073489852097177950401568476377333970545290590106007377782540002258424527629550039988395207817393754072116158892205344988532909731556149781263035042104847401378563610300473891708122024090216953246843365056183746195106883235661782512398628023983212765032808921990306665465403723029153970364656627662363482763741389227339999225082549958423571348595687840437395971355976733543903458496226172620315500993730954093139516157090481698551239561121825961463820441226732988250504800758262382");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_300_LENGTH2_300_SIGN1_PLUS_SIGN2_MINUS)
{
    BigInteger b("723085710600218062650933023587923476589623894235324510472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357453244654778967912376582");
    b.multiply("-865974983284573850973248597342896572389732589237459235769327452834737892357357326823409812079108497283450964987345678562913402135007658050090132094237476126465235676757786798009856890546097809408787512737541726847621635416573127646523675742657935769784537806784370579324523453479126734810209109091901");
    BOOST_CHECK_EQUAL(b.getNumber(), "-626174136150338041849666678564161717337444215341554788470550319933736924219468731873674467654024358677304657705415675769073489852097177950401568476377333970545290590106007377782540002258424527629550039988395207817393754072116158892205344988532909731556149781263035042104847401378563610300473891708122024090216953246843365056183746195106883235661782512398628023983212765032808921990306665465403723029153970364656627662363482763741389227339999225082549958423571348595687840437395971355976733543903458496226172620315500993730954093139516157090481698551239561121825961463820441226732988250504800758262382");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_300_LENGTH2_300_SIGN1_MINUS_SIGN2_PLUS_SWAPPED)
{
    BigInteger b("-865974983284573850973248597342896572389732589237459235769327452834737892357357326823409812079108497283450964987345678562913402135007658050090132094237476126465235676757786798009856890546097809408787512737541726847621635416573127646523675742657935769784537806784370579324523453479126734810209109091901");
    b.multiply("723085710600218062650933023587923476589623894235324510472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357453244654778967912376582");
    BOOST_CHECK_EQUAL(b.getNumber(), "-626174136150338041849666678564161717337444215341554788470550319933736924219468731873674467654024358677304657705415675769073489852097177950401568476377333970545290590106007377782540002258424527629550039988395207817393754072116158892205344988532909731556149781263035042104847401378563610300473891708122024090216953246843365056183746195106883235661782512398628023983212765032808921990306665465403723029153970364656627662363482763741389227339999225082549958423571348595687840437395971355976733543903458496226172620315500993730954093139516157090481698551239561121825961463820441226732988250504800758262382");
}

BOOST_AUTO_TEST_CASE(MULTIPLICATION_LENGTH1_300_LENGTH2_300_SIGN1_PLUS_SIGN2_MINUS_SWAPPED)
{
    BigInteger b("865974983284573850973248597342896572389732589237459235769327452834737892357357326823409812079108497283450964987345678562913402135007658050090132094237476126465235676757786798009856890546097809408787512737541726847621635416573127646523675742657935769784537806784370579324523453479126734810209109091901");
    b.multiply("-723085710600218062650933023587923476589623894235324510472079222859221398754678393418559797973621216294048724576577938038535767825619122335501093164112970424167564557439936425971731353840543886387599029948514091103121167882144867257898921128471050387920147237806845988151758357453244654778967912376582");
    BOOST_CHECK_EQUAL(b.getNumber(), "-626174136150338041849666678564161717337444215341554788470550319933736924219468731873674467654024358677304657705415675769073489852097177950401568476377333970545290590106007377782540002258424527629550039988395207817393754072116158892205344988532909731556149781263035042104847401378563610300473891708122024090216953246843365056183746195106883235661782512398628023983212765032808921990306665465403723029153970364656627662363482763741389227339999225082549958423571348595687840437395971355976733543903458496226172620315500993730954093139516157090481698551239561121825961463820441226732988250504800758262382");
}
BOOST_AUTO_TEST_SUITE_END()
