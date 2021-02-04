import './App.css';
import React, {Component} from "react";
import "bootstrap/dist/css/bootstrap.min.css";
import {Container, Row, Navbar, InputGroup, FormControl, Col, Button} from 'react-bootstrap';
import Pelak from './assets/pelak.png';
import ReserveApi from "./api";
import Swal from "sweetalert2";

class App extends Component {

    state = {
        firstSection: '',
        secondSection: '',
        thirdSection: '',
        forthSection: '',
    }

    async reserveCar() {
        const id = this.state.firstSection + this.state.secondSection + this.state.thirdSection;
        const response = await ReserveApi.reserveCar(id);
        if (response.status === 201) {
            Swal.fire({
                position: 'center',
                icon: 'success',
                title: `محل رزرو شده شما شماره ${response.data.place}  می‌باشد.`,
                showConfirmButton: true,
            })

        } else if (response.status === 400 && response.data.message === 'Bad request') {
            Swal.fire({
                position: 'center',
                icon: 'error',
                title: 'این شماره قبلا رزرو داشته است.',
                showConfirmButton: true,
            })
        } else if (response.status === 500) {
            Swal.fire({
                position: 'center',
                icon: 'warning',
                title: 'لطفا دقایقی دیگر تلاش نمایید.',
                showConfirmButton: true,
            })
        } else {
            Swal.fire({
                position: 'center',
                icon: 'warning',
                title: 'پلاک را به درستی وارد نمایید.',
                showConfirmButton: true,
            })
        }
    }


    handleChange = (name, e) => {
        const value = e.target.value;
        this.setState(prevState => {
            const newState = {...prevState};
            newState[name] = value;
            return newState;
        });
    };


    render() {
        return (
            <div>
                <div className="background"></div>
                <Navbar style={{backgroundColor: "rgba(0,0,179,0.70)"}}>
                    <Navbar.Text className="header-texts">سیستم‌های نهفته و بی‌درنگ</Navbar.Text>
                    <Navbar.Toggle/>
                    <Navbar.Collapse className="justify-content-end">
                        <Navbar.Text className="header-texts">
                            سیستم رزرو جای پارکینگ
                        </Navbar.Text>
                    </Navbar.Collapse>
                </Navbar>
                <Container className="mt-5">
                    <Row className="justify-content-center">
                        <img src={Pelak} alt="pelak"/>
                    </Row>
                    <Row className="justify-content-center align-items-center mt-4">
                        <span className="main-text">لطفا پلاک خود را وارد نمایید.</span>
                    </Row>
                    <Row className="justify-content-center id-input align-items-center mt-5">
                        <Col s={2}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="12"
                                    value={this.state.firstSection}
                                    onChange={(value) => this.handleChange('firstSection', value)}
                                />
                            </InputGroup>
                        </Col>
                        <Col s={1}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="ب"
                                    value={this.state.secondSection}
                                    onChange={(value) => this.handleChange('secondSection', value)}
                                />
                            </InputGroup>
                        </Col>
                        <Col s={3}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="365"
                                    value={this.state.thirdSection}
                                    onChange={(value) => this.handleChange('thirdSection', value)}
                                />
                            </InputGroup>
                        </Col>
                        <Col s={1}>
                            <Row className="justify-content-center">
                                <span className="main-text">ایران</span>
                            </Row>
                        </Col>
                        <Col s={2}>
                            <InputGroup className="mb-3 iransans-font">
                                <FormControl
                                    placeholder="11"
                                    value={this.state.forthSection}
                                    onChange={(value) => this.handleChange('forthSection', value)}
                                />
                            </InputGroup>
                        </Col>
                    </Row>
                    <Row className="justify-content-center align-items-center">
                        <Button disabled={
                            this.state.firstSection.length === 0
                            || this.state.secondSection.length === 0
                            || this.state.thirdSection.length === 0
                            || this.state.forthSection.length === 0
                        } onClick={() => this.reserveCar()} variant="warning"
                                className="reserve-button">رزرو</Button>
                    </Row>
                </Container>
            </div>
        );
    }
}

export default App;
