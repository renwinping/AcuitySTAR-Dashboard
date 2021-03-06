#include "summary.h"

using namespace std;


QVector<QString> Summary::getFaculty(QString tier1, QString tier2, int startDate, int endDate, QString csvtype){

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    int count=0;
    QVector<QString> facultys;
    QString field, source, date;

    if(csvtype=="Teaching"){
        date="StartDate";
        field="Program";
        startDate=tier2.toInt();
        endDate=tier2.toInt();
        if(tier1=="PME") tier1="Postgraduate Medical Education";
        else if(tier1=="CME") tier1="Continuing Medical Education";
        else if(tier1=="UME") tier1="Undergraduate Medical Education";
    }
    else if(csvtype=="Presentations"){
        date="Date";
        field="Type";
        startDate=tier2.toInt();
        endDate=tier2.toInt();
        if(tier1=="Abstracts Presented") tier1="Abstract Presented";
    }
    else if(csvtype=="Publications"){
        date="StatusDate";
        field="Type";
        if(tier2 == "Published Abstracts") tier2="Published Abstract";
        else if(tier2 == "Journal Articles") tier2="Journal Article";
        tier1=tier2;
    }
    else if(csvtype=="Grants"){
        date="StartDate";
        field="FundingType";
        if(tier2=="Peer Reviewed")
            source="PeerReviewed";
        else if(tier2=="Industry Sponsored")
            source="IndustryGrant";
    }

    if (tier1 == "Other" && csvtype == "Teaching"){
        tier1 = "NOT IN ('Postgraduate Medical Education', 'Continuing Medical Education', 'Undergraduate Medical Education')";
    }
    else if (tier1 == "Other" && csvtype == "Presentations"){
        tier1 = "NOT IN ('Invited Lectures', 'Abstracts Presented')";
    }
    else{
        tier1  = "= '" + tier1 + "'";
    }
    QString q = "SELECT DISTINCT MemberName FROM " + csvtype + " WHERE (";
    for( int a = startDate; a < endDate; a = a + 1 )
    {
        //count++;
        QString qstr=QString::number(a);
        q = q + " " + date + " LIKE '%" + qstr + "%' OR ";
    }
    QString qstr1=QString::number(endDate);
    if(csvtype=="Grants"){
        q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1 + " AND " + source + " = 'True'";
    }
    else{
        q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1;
    }
    ////cout<<q.toStdString()<<endl;
    qry.prepare(q);
    qry.exec();
    while(qry.next()){
        ////count++;
        facultys.append(qry.value(0).toString());
    }
    //cout<<"Get Faculty:"<<count<<endl;
    return facultys;
}


QVector<double> Summary::getTier3(QString tier1, QString tier2, QString tier3, int startDate, int endDate, QString csvtype){

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    int count=0;
    QVector<double> facultys;
    QString field, source, data, date;
    int values;

    if(csvtype=="Teaching"){
        date="StartDate";
        values=2;
        field="Program";
        startDate=tier2.toInt();
        endDate=tier2.toInt();
        data="SUM(TotalHours), SUM(NumberOfTrainees)";
        if(tier1=="PME") tier1="Postgraduate Medical Education";
        else if(tier1=="CME") tier1="Continuing Medical Education";
        else if(tier1=="UME") tier1="Undergraduate Medical Education";
    }
    else if(csvtype=="Presentations"){
        date="Date";
        values=1;
        field="Type";
        startDate=tier2.toInt();
        endDate=tier2.toInt();
        data="COUNT(*)";
        if(tier1=="Abstracts Presented") tier1="Abstract Presented";
    }
    else if(csvtype=="Publications"){
        date="StatusDate";
        values=1;
        field="Type";
        if(tier2 == "Published Abstracts") tier2="Published Abstract";
        else if(tier2 == "Journal Articles") tier2="Journal Article";
        tier1=tier2;
        data="COUNT(*)";
    }
    else if(csvtype=="Grants"){
        date="StartDate";
        values=2;
        field="FundingType";
        if(tier2=="Peer Reviewed")
            source="PeerReviewed";
        else if(tier2=="Industry Sponsored")
            source="IndustryGrant";
        data="COUNT(*), SUM(TotalAmount)";
    }

    if (tier1 == "Other" && csvtype == "Teaching"){
        tier1 = "NOT IN ('Postgraduate Medical Education', 'Continuing Medical Education', 'Undergraduate Medical Education')";
    }
    else if (tier1 == "Other" && csvtype == "Presentations"){
        tier1 = "NOT IN ('Invited Lectures', 'Abstracts Presented')";
    }
    else{
        tier1  = "= '" + tier1 + "'";
    }
    QString q = "SELECT "+data+" FROM "+csvtype+" WHERE (";
    for( int a = startDate; a < endDate; a = a + 1 )
    {

        QString qstr=QString::number(a);
        q = q + " " + date + " LIKE '%" + qstr + "%' OR ";
    }
    QString qstr1=QString::number(endDate);
    if(csvtype=="Grants"){
        q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1 + " AND " + source + " = 'True' AND MemberName = " + "'" + tier3 + "'";
    }
    else{
        q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1 + " AND MemberName = " + "'" + tier3 + "'";
    }
    qry.prepare(q);
    qry.exec();
    while(qry.next()){
        for(int n=0; n<values; n++){
            QCoreApplication::processEvents();
            facultys.append(qry.value(n).toDouble());
        }
    }
    return facultys;
}


QVector<double> Summary::getTier2(QString tier1, QString tier2, int startDate, int endDate, QString csvtype){

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    QVector<double> facultys;
    QString field, source, data, date;
    int values;

    if(csvtype=="Teaching"){
        date="StartDate";
        values=2;
        field="Program";
        startDate=tier2.toInt();
        endDate=tier2.toInt();
        data="SUM(TotalHours), SUM(NumberOfTrainees)";
        if(tier1=="PME") tier1="Postgraduate Medical Education";
        else if(tier1=="CME") tier1="Continuing Medical Education";
        else if(tier1=="UME") tier1="Undergraduate Medical Education";
    }
    else if(csvtype=="Presentations"){
        date="Date";
        values=1;
        field="Type";
        startDate=tier2.toInt();
        endDate=tier2.toInt();
        data="COUNT(*)";
        if(tier1=="Abstracts Presented") tier1="Abstract Presented";
    }
    else if(csvtype=="Publications"){
        date="StatusDate";
        values=1;
        field="Type";
        if(tier2 == "Published Abstracts") tier2="Published Abstract";
        else if(tier2 == "Journal Articles") tier2="Journal Article";
        tier1=tier2;
        data="COUNT(*)";
    }
    else if(csvtype=="Grants"){
        date="StartDate";
        values=2;
        field="FundingType";
        if(tier2=="Peer Reviewed")
            source="PeerReviewed";
        else if(tier2=="Industry Sponsored")
            source="IndustryGrant";
        data="COUNT(*), SUM(TotalAmount)";
    }

    if (tier1 == "Other" && csvtype == "Teaching"){
        tier1 = "NOT IN ('Postgraduate Medical Education', 'Continuing Medical Education', 'Undergraduate Medical Education')";
    }
    else if (tier1 == "Other" && csvtype == "Presentations"){
        tier1 = "NOT IN ('Invited Lectures', 'Abstracts Presented')";
    }
    else{
        tier1  = "= '" + tier1 + "'";
    }
    QString q = "SELECT "+data+" FROM "+csvtype+" WHERE (";
    for( int a = startDate; a < endDate; a = a + 1 )
    {
        QCoreApplication::processEvents();
        QString qstr=QString::number(a);
        q = q + " " + date + " LIKE '%" + qstr + "%' OR ";
    }
    QString qstr1=QString::number(endDate);
    if(csvtype=="Grants"){
        q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1 + " AND " + source + " = 'True'";
    }
    else{
        q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1;
    }
    ////cout<<q.toStdString()<<endl;
    qry.prepare(q);
    qry.exec();
    while(qry.next()){
        for(int n=0; n<values; n++){
            QCoreApplication::processEvents();
            facultys.append(qry.value(n).toDouble());
        }
    }
    ////cout<<"Tier 2:"<<count<<endl;
    return facultys;
}


QVector<double> Summary::getTier1(QString tier1, int startDate, int endDate, QString csvtype){

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    QVector<double> facultys;
    QString field, data, date;
    int values;

    if(csvtype=="Teaching"){
        date="StartDate";
        values=2;
        field="Program";
        data="SUM(TotalHours), SUM(NumberOfTrainees)";
        if(tier1=="PME") tier1="Postgraduate Medical Education";
        else if(tier1=="CME") tier1="Continuing Medical Education";
        else if(tier1=="UME") tier1="Undergraduate Medical Education";
    }
    else if(csvtype=="Presentations"){
        date="Date";
        values=1;
        field="Type";
        data="COUNT(*)";
        if(tier1=="Abstracts Presented") tier1="Abstract Presented";
    }
    else if(csvtype=="Publications"){
        date="StatusDate";
        values=1;
        field="Type";
        data="COUNT(*)";
    }
    else if(csvtype=="Grants"){
        date="StartDate";
        values=2;
        field="FundingType";
        data="COUNT(*), SUM(TotalAmount)";
    }

    if (tier1 == "Other" && csvtype == "Teaching"){
        tier1 = "NOT IN ('Postgraduate Medical Education', 'Continuing Medical Education', 'Undergraduate Medical Education')";
    }
    else if (tier1 == "Other" && csvtype == "Presentations"){
        tier1 = "NOT IN ('Invited Lectures', 'Abstracts Presented')";
    }
    else if (csvtype == "Publications"){
        tier1 = "IN ('Journal Article', 'Published Abstract', 'Books', 'Book Chapters', 'Letters to Editor')";
    }
    else{
        tier1  = "= '" + tier1 + "'";
    }
    QString q = "SELECT "+data+" FROM "+csvtype+" WHERE (";
    for( int a = startDate; a < endDate; a = a + 1 )
    {
        QCoreApplication::processEvents();
        QString qstr=QString::number(a);
        q = q + " " + date + " LIKE '%" + qstr + "%' OR ";
    }
    QString qstr1=QString::number(endDate);
    q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1;
    ////cout<<q.toStdString()<<endl;
    qry.prepare(q);
    qry.exec();
    while(qry.next()){
        for(int n=0; n<values; n++){
            QCoreApplication::processEvents();
            facultys.append(qry.value(n).toDouble());
        }
    }
    return facultys;
}


QVector<double> Summary::getTier1Filter(QString tier1, QString tier3, int startDate, int endDate, QString csvtype){

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    QVector<double> facultys;
    QString field, data, date;
    int values;

    if(csvtype=="Teaching"){
        date="StartDate";
        values=2;
        field="Program";
        data="SUM(TotalHours), SUM(NumberOfTrainees)";
        if(tier1=="PME") tier1="Postgraduate Medical Education";
        else if(tier1=="CME") tier1="Continuing Medical Education";
        else if(tier1=="UME") tier1="Undergraduate Medical Education";
    }
    else if(csvtype=="Presentations"){
        date="Date";
        values=1;
        field="Type";
        data="COUNT(*)";
        if(tier1=="Abstracts Presented") tier1="Abstract Presented";
    }
    else if(csvtype=="Publications"){
        date="StatusDate";
        values=1;
        field="Type";
        data="COUNT(*)";
    }
    else if(csvtype=="Grants"){
        date="StartDate";
        values=2;
        field="FundingType";
        data="COUNT(*), SUM(TotalAmount)";
    }

    if (tier1 == "Other" && csvtype == "Teaching"){
        tier1 = "NOT IN ('Postgraduate Medical Education', 'Continuing Medical Education', 'Undergraduate Medical Education')";
    }
    else if (tier1 == "Other" && csvtype == "Presentations"){
        tier1 = "NOT IN ('Invited Lectures', 'Abstracts Presented')";
    }
    else if (csvtype == "Publications"){
        tier1 = "IN ('Journal Article', 'Published Abstract', 'Books', 'Book Chapters', 'Letters to Editor')";
    }
    else{
        tier1  = "= '" + tier1 + "'";
    }
    QString q = "SELECT "+data+" FROM "+csvtype+" WHERE (";
    for( int a = startDate; a < endDate; a = a + 1 )
    {
        QCoreApplication::processEvents();
        QString qstr=QString::number(a);
        q = q + " " + date + " LIKE '%" + qstr + "%' OR ";
    }
    QString qstr1=QString::number(endDate);
    q = q + " " + date + " LIKE '%" + qstr1 + "%' ) AND " + field + " " + tier1 + " AND MemberName = " + "'" + tier3 + "'";
    ////cout<<q.toStdString()<<endl;
    qry.prepare(q);
    qry.exec();
    while(qry.next()){
        for(int n=0; n<values; n++){
            QCoreApplication::processEvents();
            facultys.append(qry.value(n).toDouble());
        }
    }
    return facultys;
}
