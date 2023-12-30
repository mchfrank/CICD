#!/bin/bash
scp /home/gitlab-runner/builds/ew8izr2C/0/students/DO6_CICD.ID_356283/balgruuf_student.21_school.ru/DO6_CICD-1/src/cat/s21_cat balg2@172.24.116.8:/usr/local/bin
scp /home/gitlab-runner/builds/ew8izr2C/0/students/DO6_CICD.ID_356283/balgruuf_student.21_school.ru/DO6_CICD-1/src/grep/s21_grep balg2@172.24.116.8:/usr/local/bin
ssh balg2@172.24.116.8 ls -lah /usr/local/bin
